#version 460 core

layout(location = 0) in vec3 a_Position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}