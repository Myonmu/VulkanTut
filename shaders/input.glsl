layout(set = 0, binding = 0) uniform SceneUbo {
    mat4 view;
    mat4 proj;
    vec4 camProps;
    vec4 camPos;
    vec4 viewDir;
} sceneUbo;

layout(set = 0, binding = 1) uniform MainLight{
    vec4 direction;
    vec4 color;
} mainLight;