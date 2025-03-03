#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform bool u_Blinn;

void main()
{
    vec3 color = texture(u_Texture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.5 * color;

    // diffuse
    vec3 lightDir = normalize(u_LightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diffuseFactor = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diffuseFactor * color;

    // specular
    vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specFactor = 0.0;

    if(u_Blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specFactor = pow(max(dot(normal, halfwayDir), 0.0), 32.0); // spec shininess exp has to be set a bit higher than Phong to get similar results
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * specFactor; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}