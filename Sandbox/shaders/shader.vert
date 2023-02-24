#version 450

//Vertex DATA 
layout(binding = 0, location = 0) in vec3 inPosition;
layout(binding = 0, location = 1) in vec3 inColor;
layout(binding = 0, location = 2) in vec2 inTexCoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 proj;
} projectionMx;

layout(set = 2, binding = 0) uniform UniformBufferObject {
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