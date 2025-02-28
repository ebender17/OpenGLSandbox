#version 460 core

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

uniform float u_FromRefractionIndex;
uniform float u_ToRefractionIndex;
uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{
    float ratio = u_FromRefractionIndex / u_ToRefractionIndex;
    vec3 viewDir = normalize(v_Position - u_CameraPos);
    vec3 refraction = refract(viewDir, normalize(v_Normal), ratio);
    color = vec4(texture(u_Skybox, refraction).rgb, 1.0);
}