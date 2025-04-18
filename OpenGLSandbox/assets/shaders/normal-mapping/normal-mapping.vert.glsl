#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};

uniform mat4 u_Model;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

out VS_OUT {
    vec3 FragWorldPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

void main()
{
    vs_out.FragWorldPos = vec3(u_Model * vec4(a_Position, 1.0));   
    vs_out.TexCoords = a_TexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 N = normalize(normalMatrix * a_Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * u_LightPos;
    vs_out.TangentViewPos  = TBN * u_ViewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragWorldPos;
        
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}