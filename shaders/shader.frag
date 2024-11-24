#version 450

#extension GL_GOOGLE_include_directive : require
#include "input.glsl"

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inWorldPos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPosition;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outAlbedo;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

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

    vec3 N = normalize(inNormal);
    N.y = -N.y;
    outNormal = vec4(N, 1.0);

    outAlbedo.rgb = inColor;

    // Store linearized depth in alpha component
    outPosition.a = linearDepth(gl_FragCoord.z);

    // Write color attachments to avoid undefined behaviour (validation error)
    outColor = vec4(0.0);
}