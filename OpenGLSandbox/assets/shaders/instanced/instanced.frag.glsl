#version 460 core

out vec4 FragColor;

layout (location = 0) in vec3 v_Color;

uniform sampler2D u_Texture;

void main()
{
    FragColor = vec4(v_Color, 1.0);
}