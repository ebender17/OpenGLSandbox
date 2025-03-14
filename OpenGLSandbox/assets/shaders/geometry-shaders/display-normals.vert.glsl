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
    vec3 Normal;
} vs_out;

uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_View * u_Model * vec4(a_Position, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(u_View * u_Model)));
    vs_out.Normal = normalize(vec3(vec4(normalMatrix * a_Normal, 0.0)));
}