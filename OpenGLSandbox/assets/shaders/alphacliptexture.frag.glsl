#version 460

layout (location = 0) out vec4 color;

layout (location = 0) in vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform float u_AlphaThreshold;
uniform vec4 u_Color;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoords);
    if (texColor.a < u_AlphaThreshold)
        discard;
    color = u_Color * texColor;
}