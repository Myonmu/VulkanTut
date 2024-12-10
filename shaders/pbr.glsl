/** GLOSSARY OF TERMS AND NOTATIONS

V : view vector (from shading point to camera)
L : light vector (from shading point to light source)
R : reflection vector (reflective vector of L along surface normal)
N : macrofacet normal (which is found in normal maps)
H : half-way vector ( normalize(V + L) )
X : tangent vector
Y : bitangent vector

*/

// Utilities

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef TWO_PI
#define TWO_PI 6.283185307
#endif

#ifndef INV_PI
#define INV_PI 0.3183098861
#endif

#ifndef SQRT_2_ON_PI
#define SQRT_2_ON_PI 0.7978845608
#endif

#ifndef ETA
#define ETA 0.0001
#endif

#define POSITIVE(v) max(v, 0.0)

#define SQR(v) v*v


#define ETA_RETURN_ZERO(v)\
    if(v < ETA) return 0;


vec3 calcReflectionVector(vec3 lightIn, vec3 normal){
    return 2.0*(dot(normal, lightIn))*normal - lightIn;
}

vec3 calcHalfwayVector(vec3 view, vec3 light){
    return normalize(view + light);
}

float roughnessToGlossiness(float roughness){
    return 1.0 - roughness;
}

//============================ Diffuse ===============================

vec3 Diffuse_Lambert(vec3 color){
    return color * INV_PI;
}

vec3 Diffuse_Burley(float NdotV, float NdotL, float VdotH, vec3 color, float roughness ){
    float FD90 = 0.5 + 2* SQR(VdotH) * roughness;
    float FdV = 1.0 + (FD90 - 1.0) * pow(1.0 - NdotV, 5.0);
    float FdL = 1.0 + (FD90 - 1.0) * pow(1.0 - NdotL, 5.0);
    return color * (INV_PI * FdV * FdL);
}

vec3 Diffuse_OrenNayar(float NdotV, float NdotL, float VdotH, vec3 color, float roughness ){
    float a = SQR(roughness);
	float s = a;
	float s2 = SQR(s);
	float VdotL = 2 * SQR(VdotH) - 1;		// double angle identity
	float Cosri = VdotL - NdotV * NdotL;
	float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
	float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * ( Cosri >= 0 ? 1 / max( NdotL, NdotV )  : 1 );
	return color / PI * ( C1 + C2 ) * ( 1.0 + roughness * 0.5 );
}

//============================ Normal Distribution functions (NDF) ============================

// Phong, and Blinn-Phong assume a specular/glossimess workflow.

float NDF_Phong(float RdotV, float specularPower, float specularGlossiness){
    float dist = pow(RdotV, specularGlossiness) * specularPower;
    dist *= (2.0 + specularPower) / TWO_PI;
    return dist;
}

float NDF_BlinnPhong(float NdotH, float specularPower, float specularGlossiness){
    // the formula is unchanged, only RdotV is substituted with NdotH
    return NDF_Phong(NdotH, specularPower, specularGlossiness);
}

// The following NDF all use metalic/roughness

float NDF_Beckmann(float NdotH, float roughness){
    float ndothSqr = SQR(NdotH);
    float roughnessSqr = SQR(roughness);
    return exp((ndothSqr - 1)/(roughnessSqr * ndothSqr)) / (PI*roughnessSqr * SQR(ndothSqr));
}

float NDF_Gaussian(float NdotH, float roughness){
    float roughnessSqr = SQR(roughness);
    float thetaH = acos(NdotH);
    return exp(-thetaH*thetaH/roughnessSqr);
}

float NDF_GGX(float NdotH, float roughness){
    float roughnessSqr = SQR(roughness);
    float ndothSqr = SQR(NdotH);
    float tanDotHSqr = (1.0 - ndothSqr)/ndothSqr;
    return INV_PI * SQR(roughness/(ndothSqr * (roughnessSqr + tanDotHSqr)));
}

float NDF_TrowbridgeReitz(float NdotH, float roughness){
    NdotH = clamp(NdotH, 0.0, 1.0);
    float roughnessSqr = SQR(roughness);
    float dist = NdotH * (NdotH * roughnessSqr - NdotH) + 1.0;
    return roughnessSqr / (PI * SQR(dist));
}

float NDF_Anisotropic_TrowbridgeReitz(float NdotH, float HdotX, float HdotY,
float anisotropy, float roughness, vec2 axialMultiplier){
    float aspect = sqrt(1.0 - anisotropy*0.9);
    float x = max(ETA, SQR(roughness/aspect)) * axialMultiplier.x;
    float y = max(ETA, SQR(roughness*aspect)) * axialMultiplier.y;
    return INV_PI/(x*y*(SQR(SQR(HdotX/x) + SQR(HdotY/y) + SQR(NdotH ))));
}

float NDF_Anisotropic_Ward(float NdotL, float NdotV, float NdotH, float HdotX, float HdotY,
float anisotropy, float roughness, vec2 axialMultiplier){
    float aspect = sqrt(1.0 - anisotropy*0.9);
    float x = max(ETA, SQR(roughness/aspect)) * axialMultiplier.x;
    float y = max(ETA, SQR(roughness*aspect)) * axialMultiplier.y;
    float e = -(SQR(SQR(HdotX/x) + SQR(HdotY/y) + SQR(NdotH )));
    float dist = 1.0/(4.0*PI*x*y*sqrt(NdotL * NdotV));
    dist *= exp(e);
    return dist;
}

//=========================== Geometric Shadowing Functions (GSF) ===============================

float GSF_Implicit(float NdotL, float NdotV){
    return POSITIVE(NdotL) * POSITIVE(NdotV);
    //return 0.25; // constant in UE
}

float GSF_Anisotropic_AshikhminShirley(float NdotL, float NdotV, float LdotH){
    return NdotL*NdotV/(LdotH*max(NdotL,NdotV));
}

float GSF_AshikhminPremoze(float NdotL, float NdotV){
    return NdotL*NdotV/(NdotL + NdotV - NdotL*NdotV);
}

float GSF_Duer(vec3 light, vec3 view, vec3 normal, float NdotL, float NdotV){
    vec3 LpV = light + view;
    return dot(LpV, LpV) * pow(dot(LpV, normal), -4.0);
}

float GSF_Anisotropic_Neumann(float NdotL, float NdotV){
    return (NdotL * NdotV) / max(NdotL, NdotV);
}

float GSF_Kelemen(float NdotL, float NdotV, float roughness){
    float k = SQR(roughness) * SQRT_2_ON_PI;
    float gh = NdotV * k + (1.0 - k);
    return SQR(gh) * NdotL;
}

float GSF_CookTorrance(float NdotL, float NdotV, float VdotH, float NdotH){
    return min(1.0, min(2.0*NdotH*NdotV/VdotH, 2.0*NdotH*NdotL/VdotH));
}

float GSF_Ward(float NdotL, float NdotV){
    return pow(NdotL*NdotV, 0.5);
}

float GSF_Anisotropic_Kurt(float NdotL, float NdotV, float VdotH, float roughness){
    return NdotL*NdotV/(VdotH*pow(NdotL*NdotV, roughness));
}

float GSF_Walter(float NdotL, float NdotV, float roughness){
    float roughnessSqr = SQR(roughness);
    float ndotlSqr = SQR(NdotL);
    float ndotvSqr = SQR(NdotV);

    float sl = 2.0 / (1.0 + sqrt(1.0 + roughnessSqr * (1.0 - ndotlSqr)/(ndotlSqr)));
    float sv = 2.0/ (1.0 + sqrt(1.0 + roughnessSqr * (1.0 - ndotvSqr)/(ndotvSqr)));

    return sl * sv;
}

float GSF_SmithBeckman(float NdotL, float NdotV, float roughness){
    float roughnessSqr = SQR(roughness);
    float ndotlSqr = SQR(NdotL);
    float ndotvSqr = SQR(NdotV);

    float l = NdotL / (roughnessSqr * sqrt(1 - ndotlSqr));
    float v = NdotV / (roughnessSqr * sqrt(1 - ndotvSqr));

    float sl = l < 1.6 ? (3.535 * l + 2.181 * SQR(l))/(1+2.276*l+2.577*SQR(l)) : 1.0;
    float sv = v < 1.6 ? (3.535 * v + 2.181 * SQR(v))/(1+2.276*v+2.577*SQR(v)) : 1.0;

    return sl*sv;
}

// Walter but multiplied by NdotV/NdotV (less divisions)
float GSF_GGX(float NdotL, float NdotV, float roughness){
    float roughnessSqr = SQR(roughness);
    float ndotlSqr = SQR(NdotL);
    float ndotvSqr = SQR(NdotV);

    float sl = 2.0 * NdotL / (NdotL + sqrt(roughnessSqr + (1.0- roughnessSqr)*ndotlSqr));
    float sv = 2.0 * NdotV / (NdotV + sqrt(roughnessSqr + (1.0- roughnessSqr)*ndotvSqr));

    return sl*sv;
}

// Approx. of Smith
float GSF_Schlick(float NdotL, float NdotV, float roughness){
    float roughnessSqr = SQR(roughness);
    float sl = NdotL/(NdotL * (1.0 - roughnessSqr) + roughnessSqr);
    float sv = NdotV/(NdotV * (1.0 - roughnessSqr) + roughnessSqr);
    return sl*sv;
}

// Approx. of Beckman
float GSF_SchlickBeckman(float NdotL, float NdotV, float roughness){
    float k = SQR(roughness)*SQRT_2_ON_PI;
    float sl = NdotL/(NdotL*(1.0-k) + k);
    float sv = NdotV/(NdotV*(1.0-k) + k);
    return sl*sv;
}

// Approx. of GGX
float GSF_SchlickGGX(float NdotL, float NdotV, float roughness){
    float k = roughness * 0.5;
    float sl = POSITIVE(NdotL)*(1.0-k) + k;
    float sv = POSITIVE(NdotV)*(1.0-k) + k;
    return GSF_Implicit(NdotL, NdotV)/(sl*sv);
}

//====================== Fresnel Functions (FF) ===========================

float SchlickFresnelPow5(float i){
    float x = clamp(1.0 - i, 0.0, 1.0);
    float xSqr = SQR(x);
    return SQR(xSqr)*x; // x^5
}

vec3 FF_F0(vec3 albedo, float metalness){
    return mix(vec3(0.04), albedo, metalness);
}

vec3 FF_Schlick(float VdotH, vec3 albedo, float metalness){
    float f = SchlickFresnelPow5(POSITIVE(VdotH));
    vec3 F0 = FF_F0(albedo, metalness);
    return F0 + (1.0 - F0) * f;
}

float FF_SchlickIOR(float LdotH, float ior){
    float f0 = SQR(ior-1)/SQR(ior+1);
    return f0 + (1.0-f0) * SchlickFresnelPow5(LdotH);
}

vec3 FF_SphericalGaussian(float LdotH, vec3 specularColor){
    float power = ((-5.55473 * POSITIVE(LdotH)) - 6.98316) * POSITIVE(LdotH);
    return specularColor + (1.0 - specularColor) * pow(2.0, power);
}


//=============== BRDF ====================

vec3 BRDF_Specular(float NdotL, float NdotV, float NDF, vec3 FF, float GSF){
    return NDF * GSF * FF / ((4.0 * (POSITIVE(NdotL) * POSITIVE(NdotV))) + ETA);
}

