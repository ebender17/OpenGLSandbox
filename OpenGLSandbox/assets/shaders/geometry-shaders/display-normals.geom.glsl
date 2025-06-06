#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in Varyings {
    vec3 Normal;
} gs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 u_Projection;

void generateLine(int index)
{
    gl_Position = u_Projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = u_Projection * (gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generateLine(0);
    generateLine(1);
    generateLine(2);
}