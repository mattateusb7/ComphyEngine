#version 450

layout (location = 0) out vec4 outputColor;

in vec3 v_Position;

void main()
{
    outputColor = vec4(v_Position*0.5 + 0.5, 1.0);
}