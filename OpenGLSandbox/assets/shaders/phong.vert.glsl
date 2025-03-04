#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out Varyings {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};
uniform mat4 u_Model;

void main()
{
    vs_out.TexCoords = a_TexCoord;
    vs_out.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    gl_Position = u_ViewProjection * vec4(vs_out.FragPos, 1.0);
}