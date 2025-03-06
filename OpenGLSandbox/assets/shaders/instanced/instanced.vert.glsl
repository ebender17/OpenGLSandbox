#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 3) in mat4 a_InstanceMatrix;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_ViewProjection; 
};

void main()
{
    gl_Position = u_ViewProjection * a_InstanceMatrix * vec4(a_Position, 1.0f); 
}