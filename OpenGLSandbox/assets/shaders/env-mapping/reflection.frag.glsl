#version 460 core

layout (location = 0) out vec4 color;

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec3 v_Normal;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{
    vec3 viewDir = normalize(v_Position - u_CameraPos);
    vec3 reflection = reflect(viewDir, normalize(v_Normal));
    color = vec4(texture(u_Skybox, reflection).rgb, 1.0);
}