#version 460 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_HDRBuffer;
uniform sampler2D u_BloomBlurBuffer;
uniform bool u_HDR;
uniform bool u_Bloom;
uniform float u_Exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_HDRBuffer, v_TexCoords).rgb;
    vec3 bloomColor = texture(u_BloomBlurBuffer, v_TexCoords).rgb;
    if (u_Bloom)
    {
        hdrColor += bloomColor; // additive blending
    }
    if (u_HDR)
    {
        // reinhard tone-mapping
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));

        // exposure - higher values darker areas, lower values bright areas
        vec3 result = vec3(1.0) - exp(-hdrColor * u_Exposure);

        // gamma correction
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else
    {
        // gamma correction
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}