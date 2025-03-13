#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 u_LightPositions[4];
uniform vec3 u_LightColors[4];
uniform sampler2D u_Texture;
uniform vec3 u_ViewPos;
uniform bool u_Blinn;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(u_ViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specFactor = 0.0;

    if(u_Blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specFactor = pow(max(dot(normal, halfwayDir), 0.0), 64.0); // spec shininess exp has to be set a bit higher than Phong to get similar results
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    }

    vec3 specular = specFactor * lightColor;

    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (distance * distance);
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

void main()
{
    vec3 color = texture(u_Texture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.5 * color;

    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        lighting += BlinnPhong(fs_in.Normal, fs_in.FragPos, u_LightPositions[i], u_LightColors[i]);
    }
    color *= lighting;

    FragColor = vec4(color, 1.0);
}