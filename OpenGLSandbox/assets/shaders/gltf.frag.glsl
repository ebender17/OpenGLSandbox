#version 460

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoords);
}