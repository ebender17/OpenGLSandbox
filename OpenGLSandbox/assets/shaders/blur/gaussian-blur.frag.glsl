out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_Image;
uniform bool u_Horizontal;
uniform float u_Weights[5];

void main()
{
    vec2 texOffset = 1.0 / textureSize(u_Image, 0); // gets size of single texel
    vec3 result = texture(u_Image, v_TexCoords).rgb * u_Weights[0];
    if (u_Horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Image, v_TexCoords + vec2(texOffset.x * i, 0.0)).rgb * u_Weights[i];
            result += texture(u_Image, v_TexCoords - vec2(texOffset.x * i, 0.0)).rgb * u_Weights[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Image, v_TexCoords + vec2(0.0, texOffset.y * i)).rgb * u_Weights[i];
            result += texture(u_Image, v_TexCoords - vec2(0.0, texOffset.y * i)).rgb * u_Weights[i];
        }
    }
    FragColor = vec4(result, 1.0);
}