#version 460 core

layout (location = 0) out vec4 color;

in Varyings {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, fs_in.TexCoords);
}