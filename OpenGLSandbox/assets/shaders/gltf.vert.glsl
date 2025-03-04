#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord0;
layout(location = 3) in vec2 a_TexCoord1;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec4 a_Tangent;
layout(location = 6) in ivec4 a_Joints;
layout(location = 7) in vec4 a_Weights;

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
    vs_out.FragPos = a_Position;
    vs_out.TexCoords = a_TexCoord0;
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}