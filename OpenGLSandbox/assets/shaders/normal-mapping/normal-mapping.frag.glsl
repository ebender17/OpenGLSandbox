#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragWorldPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

void main()
{           
    vec3 normal = texture(u_NormalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 color = texture(u_DiffuseMap, fs_in.TexCoords).rgb;
    vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}