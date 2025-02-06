#version 460

layout (location = 0) out vec4 color;

const int POST_PROCESS_INVERSION = 0;
const int POST_PROCESS_GRAYSCALE = 1;
const int POST_PROCESS_SHARPEN = 2;
const int POST_PROCESS_BLUR = 3;

const float offset = 1.0 / 300.0;  

layout (location = 0) in vec2 v_TexCoords;

uniform sampler2D u_ScreenTexture;
uniform int u_PostProcessOption;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
    vec3 col = texture(u_ScreenTexture, v_TexCoords).rgb;
    switch(u_PostProcessOption) {
        case POST_PROCESS_INVERSION:
            col = 1 - col;
            break;
        case POST_PROCESS_GRAYSCALE:
            float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
            col = vec3(average, average, average);
            break;
        case POST_PROCESS_SHARPEN:
            float kernel1[9] = float[](
                -1, -1, -1,
                -1,  9, -1,
                -1, -1, -1
            );
            vec3 sampleTex1[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex1[i] = vec3(texture(u_ScreenTexture, v_TexCoords.st + offsets[i]));
            }
            col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                col += sampleTex1[i] * kernel1[i];
            break;
        case POST_PROCESS_BLUR:
            float kernel2[9] = float[](
                1.0 / 16, 2.0 / 16, 1.0 / 16,
                2.0 / 16, 4.0 / 16, 2.0 / 16,
                1.0 / 16, 2.0 / 16, 1.0 / 16  
            );
            vec3 sampleTex2[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex2[i] = vec3(texture(u_ScreenTexture, v_TexCoords.st + offsets[i]));
            }
            col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                col += sampleTex2[i] * kernel2[i];
            break;
        default:
            color = vec4(1.0, 1.0, 1.0, 1.0);
            break;
    }
    color = vec4(col, 1.0);
}