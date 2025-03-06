#version 460 core

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}