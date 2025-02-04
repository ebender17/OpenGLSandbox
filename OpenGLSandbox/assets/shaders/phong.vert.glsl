#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

layout (location = 0) out vec2 v_TexCoords;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_FragPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    v_TexCoords = a_TexCoord;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}