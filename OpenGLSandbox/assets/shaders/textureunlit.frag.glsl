#version 460 core

layout (location = 0) out vec4 color;

layout (location = 0) in vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;

void main()
{
    color = texture(u_Texture, u_TilingFactor * v_TexCoords);
}