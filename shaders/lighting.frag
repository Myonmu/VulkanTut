#version 450

#extension GL_GOOGLE_include_directive: require
#include "input.glsl"
#include "normal_mapping.glsl"
#include "pbr.glsl"
#include "color.glsl"

layout (input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, set = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, set = 1, binding = 2) uniform subpassInput inputAlbedo;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outColor;

void main()
{
    // Read G-Buffer values from previous sub pass
    vec4 fragPosTex = subpassLoad(inputPosition);
    vec4 normalTex = subpassLoad(inputNormal);
    vec4 albedoTex = subpassLoad(inputAlbedo);

    float linearDepth = fragPosTex.a;
    vec3 albedo = albedoTex.rgb;
    float roughness = albedoTex.a * albedoTex.a;
    vec3 normal = unpackNormal(normalTex.rgb);
    float metallic = normalTex.a;
    vec3 pos = fragPosTex.rgb; // todo: this can be reconstructed from hclip (will free up 3 channels in gbuffer1)

    // shading convention: vectors pointing from shading point outwards
    vec3 view = normalize(sceneUbo.camPos.rgb - pos);
    vec3 light = normalize(-mainLight.direction.rgb);
    vec3 halfway = calcHalfwayVector(view, light);

    float NdotL = dot(normal, light);
    float NdotV = dot(normal, view);
    float NdotH = dot(normal, halfway);
    float VdotH = dot(view, halfway);
    float ndf_ggx = NDF_TrowbridgeReitz(NdotH, roughness);
    float gsf_ggx = GSF_SchlickGGX(NdotL, NdotV, roughness);
    vec3 ff_schlick = FF_Schlick(VdotH, mainLight.color.rgb, metallic);

    vec3 specular = BRDF_Specular(NdotL, NdotV, ndf_ggx, ff_schlick, gsf_ggx);

    vec3 diffuseColor = Diffuse_Lambert(albedo);

    #define ambient 0.03f

    vec3 kS = ff_schlick;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    vec3 fragcolor = albedo.rgb * ambient +
    (kD * diffuseColor + specular) * mainLight.color.rgb * mainLight.color.a * POSITIVE(NdotL);

    //fragcolor = linearTosRGB(fragcolor);
    outColor = vec4(clamp(fragcolor, 0.0, 1.0), 1.0);
}