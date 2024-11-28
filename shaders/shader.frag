#version 450

#extension GL_GOOGLE_include_directive : require
#include "input.glsl"
#include "normal_mapping.glsl"

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inTexCoord;
layout (location = 4) in vec3 inTangent;
layout (location = 5) in vec3 inBitangent;


layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPosition;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outAlbedo;

layout(set = 1, binding = 0) uniform sampler2D baseColorSampler;
layout(set = 1, binding = 1) uniform sampler2D normalSampler;
layout(set = 1, binding = 2) uniform sampler2D roughnessSampler;

float linearDepth(float depth)
{
    float z = depth * 2.0f - 1.0f;
    float nearPlane = sceneUbo.camProps.x;
    float farPlane = sceneUbo.camProps.y;
    return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    outPosition = vec4(inWorldPos, 1.0);

    // Construct the TBN matrix
    mat3 TBN = mat3(normalize(inTangent), normalize(inBitangent), normalize(inNormal));

    // Sample the normal map (tangent space normal)
    vec3 tangentSpaceNormal = normalize(unpackNormal(texture(normalSampler, inTexCoord).xyz));

    // Transform the normal from tangent space to world space using the TBN matrix
    vec3 worldSpaceNormal = normalize(TBN * tangentSpaceNormal);

    // gl -> vulkan
    worldSpaceNormal.y = -worldSpaceNormal.y;
    outNormal = vec4(packNormal(worldSpaceNormal), 1.0);

    // alpha = roughness
    outAlbedo = vec4(texture(baseColorSampler, inTexCoord).rgb, texture(roughnessSampler, inTexCoord).r);


    // Store linearized depth in alpha component
    outPosition.a = linearDepth(gl_FragCoord.z);

    // Write color attachments to avoid undefined behaviour (validation error)
    outColor = vec4(0.0);
}