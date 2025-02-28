#version 460 core

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_TexCoords;

uniform samplerCube u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoords);
}