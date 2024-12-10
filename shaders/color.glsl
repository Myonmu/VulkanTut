#ifndef SRGB_TO_LINEAR_THRESHOLD
#define SRGB_TO_LINEAR_THRESHOLD 0.04045
#endif

#ifndef LINEAR_TO_SRGB_THRESHOLD
#define LINEAR_TO_SRGB_THRESHOLD 0.0031308
#endif

#ifndef GAMMA_EXP
#define GAMMA_EXP 2.4
#endif

float sRGBToLinear(float srgb){
    return mix(srgb / 12.92, pow((srgb + 0.055) / 1.055, GAMMA_EXP), step(SRGB_TO_LINEAR_THRESHOLD, srgb));
}

vec3 sRGBToLinear(vec3 srgb){
    return mix(srgb / 12.92, pow((srgb + 0.055) / 1.055, vec3(GAMMA_EXP)), step(SRGB_TO_LINEAR_THRESHOLD, srgb));
}

float linearTosRGB(float linear){
    return mix(linear * 12.92, pow(linear, 1.0 / GAMMA_EXP) * 1.055 - 0.055, step(LINEAR_TO_SRGB_THRESHOLD, linear));
}

vec3 linearTosRGB(vec3 linear){
    return mix(linear * 12.92, pow(linear, vec3(1.0 / GAMMA_EXP)) * 1.055 - 0.055, step(LINEAR_TO_SRGB_THRESHOLD, linear));
}