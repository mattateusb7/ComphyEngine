#version 450
layout (location = 0) in vec3 aPos; 

out gl_PerVertex
{
    vec4 gl_Position;
};

out vec3 v_Position;

void main()
{
    v_Position = aPos;
    gl_Position = vec4(aPos, 1.0);
}