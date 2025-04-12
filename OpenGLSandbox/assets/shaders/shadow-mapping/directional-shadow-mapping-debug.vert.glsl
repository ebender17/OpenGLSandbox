#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position, 1.0);
}