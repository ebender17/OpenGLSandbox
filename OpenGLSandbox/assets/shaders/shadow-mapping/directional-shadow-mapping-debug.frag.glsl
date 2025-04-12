#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_DepthMap;

void main()
{
    // orthographic matrix (directional shadow maps)
    float depthValue = texture(u_DepthMap, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
} 