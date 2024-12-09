#version 450

#extension GL_GOOGLE_include_directive: require
#include "input.glsl"
#include "normal_mapping.glsl"
#include "color.glsl"

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inWorldPos;
layout (location = 3) in vec2 inTexCoord;
layout (location = 4) in vec3 inTangent;


layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPosition;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outAlbedo;

layout (set = 1, binding = 0) uniform sampler2D baseColorSampler;
layout (set = 1, binding = 1) uniform sampler2D normalSampler;
layout (set = 1, binding = 2) uniform sampler2D roughnessSampler;
layout (set = 1, binding = 3) uniform PerMaterialUbo {
    /** r = metallic
    */
    vec4 pbrProps;
} perMaterialUbo;

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

    vec3 t = normalize(inTangent);
    vec3 n = normalize(inNormal);
    vec3 b = normalize(cross(inNormal, inTangent));
    // Construct the TBN matrix
    mat3 TBN = mat3(t, b, n);

    // Sample the normal map (tangent space normal)
    vec3 tangentSpaceNormal = normalize(unpackNormal(texture(normalSampler, inTexCoord).xyz));

    // Transform the normal from tangent space to world space using the TBN matrix
    vec3 worldSpaceNormal = normalize(TBN * tangentSpaceNormal);

    // gl -> vulkan // actually, depends on how the normal map was made (pas via ubo or some other magick)
    //worldSpaceNormal.y = -worldSpaceNormal.y;
    outNormal = vec4(packNormal(worldSpaceNormal), clamp(perMaterialUbo.pbrProps.r, 0.0, 1));

    // alpha = roughness
    vec3 baseColor = texture(baseColorSampler, inTexCoord).rgb;
    float roughness = texture(roughnessSampler, inTexCoord).r;
    outAlbedo = vec4(baseColor, roughness);


    // Store linearized depth in alpha component
    outPosition.a = linearDepth(gl_FragCoord.z);

    // Write color attachments to avoid undefined behaviour (validation error)
    outColor = vec4(0.0);
}