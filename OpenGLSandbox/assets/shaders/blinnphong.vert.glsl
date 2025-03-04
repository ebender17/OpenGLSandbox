#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};

void main()
{
    vs_out.FragPos = a_Position;
    vs_out.Normal = a_Normal;
    vs_out.TexCoords = a_TexCoords;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}