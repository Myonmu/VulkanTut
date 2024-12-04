#version 450

#extension GL_GOOGLE_include_directive : require
#include "input.glsl"
#include "normal_mapping.glsl"

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

    vec3 albedo = albedoTex.rgb;
    float roughness = albedoTex.a;
    vec3 normal = unpackNormal(normalTex.rgb);
    vec3 pos = fragPosTex.rgb;

    #define ambient 0.1f

    // Ambient part
    vec3 fragcolor  = albedo.rgb * ambient;

    vec3 L = mainLight.direction.rgb;

    vec3 N = normalize(normal);
    float NdotL = max(0.0, dot(N, L));
    vec3 diff = mainLight.color.rgb * albedo.rgb * NdotL * 1;

    fragcolor += diff;

    outColor = vec4(fragcolor, 1.0);
}