#version 450

//Vertex DATA 
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

//resources

layout(set = 2, binding = 0) uniform UniformBufferObject1 {
    mat4 proj;
} projectionMx;

layout(set = 2, binding = 2) uniform UniformBufferObject2 {
    mat4 model;
    mat4 view;
} modelViewMx;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = projectionMx.proj * modelViewMx.view * modelViewMx.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}