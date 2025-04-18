#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

out VS_OUT {
    vec3 FragWorldPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};

uniform mat4 u_Model;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
    vs_out.FragWorldPos = vec3(u_Model * vec4(a_Position, 1.0));
    vs_out.TexCoords = a_TexCoords;

    vec3 T = normalize(mat3(u_Model) * a_Tangent);
    vec3 B = normalize(mat3(u_Model) * a_Bitangent);
    vec3 N = normalize(mat3(u_Model) * a_Normal);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos = TBN * u_LightPos;
    vs_out.TangentViewPos = TBN * u_ViewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragWorldPos;
}