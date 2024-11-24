#version 450

#extension GL_GOOGLE_include_directive : require
#include "input.glsl"

layout( push_constant ) uniform constants
{
    mat4 model;
} pushConstants;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outWorldPos;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = sceneUbo.proj * sceneUbo.view * pushConstants.model * pos;

    // Vertex position in world space
    outWorldPos = vec3(pushConstants.model * pos);
    // GL to Vulkan coord space
    outWorldPos.y = -outWorldPos.y;

    // Normal in world space
    mat3 mNormal = transpose(inverse(mat3(pushConstants.model)));
    outNormal = mNormal * normalize(inNormal);

    // Currently just vertex color
    outColor = inColor;
}