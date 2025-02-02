#version 460

layout (location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct DirLight { 
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (location = 0) in vec2 v_TexCoords;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_FragPos;

uniform Material u_Material;

// Light uniforms
uniform vec3 u_ViewPos;
uniform float u_Time;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[4];
uniform SpotLight u_SpotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    vec3 result = CalcDirLight(u_DirLight, norm, viewDir);

    for (int i = 0; i < 4; i++)
    {
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    result += CalcSpotLight(u_SpotLight, norm, v_FragPos, viewDir);
    color = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specularMap = vec3(texture(u_Material.specular, v_TexCoords));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    vec3 emission = vec3(0.0);
    if (specularMap.r == 0.0) // check for mask
    {
        emission = texture(u_Material.emission, v_TexCoords).rgb;
        emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0;  // fading
    }

    vec3 ambient  = light.ambient  * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * specularMap;
    return (ambient + diffuse + specular + emission);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specularMap = vec3(texture(u_Material.specular, v_TexCoords));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * specularMap;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, v_TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, v_TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}