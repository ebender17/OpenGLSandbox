#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out vec2 v_TexCoords;

layout (std140, binding = 0) uniform Matrices // binding layout specifier available OpenGL v4.2+
{
    mat4 u_ViewProjection; 
};
uniform mat4 u_Model;

void main()
{
    v_TexCoords = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}