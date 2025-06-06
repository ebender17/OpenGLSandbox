#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 WorldFragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Light u_Lights[4];
uniform sampler2D u_DiffuseTexture;
uniform vec3 u_ViewPos;

void main()
{
    vec3 color = texture(u_DiffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 ambient = 0.1 * color;
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 4; i++)
    {
        vec3 lightDir = normalize(u_Lights[i].Position - fs_in.WorldFragPos);
        float diffFactor = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = u_Lights[i].Color * diffFactor * color;
        vec3 result = diffuse;
        // attenuation (use quadratic, physically based equation, as we have gamma correction)
        float distance = length(fs_in.WorldFragPos - u_Lights[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;
    }
    FragColor = vec4(ambient + lighting, 1.0);
}