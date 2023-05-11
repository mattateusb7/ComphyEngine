#version 450

//Vertex DATA 
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

//resources

layout(set = 2, binding = 0) uniform UniformBuffer0 {
    mat4 data;
} viewProjectionMx;

layout(set = 2, binding = 2) uniform UniformBuffer1{
    mat4 data;
} modelMx;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = viewProjectionMx.data * modelMx.data * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}