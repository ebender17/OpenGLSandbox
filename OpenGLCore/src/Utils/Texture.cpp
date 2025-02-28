#include "glpch.h"
#include "Texture.h"

#include <stb_image.h>

#include "Core/Core.h"

namespace OpenGLCore::Utils {

    void GenerateTexture2D(const std::string& filepath, GLuint* texture, GLint wrapOption, GLint filteringOption)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);

        // TODO : support textures with 16 and 32 bit depth per channel
        stbi_uc* data = nullptr;
        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        OPENGLCORE_ASSERT(data, "Failed to load image.");

        GLenum internalFormat, dataFormat;
        switch (channels)
        {
        case 1:
            internalFormat = GL_R8;
            dataFormat = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            dataFormat = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        default:
            OPENGLCORE_ASSERT(false, "Do not support textures with more than 4 channels.");
        }

        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringOption);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringOption);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }

    void GenerateTextureFramebuffer(GLuint* texture, unsigned int width, unsigned int height, GLint filteringOption)
    {
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filteringOption);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filteringOption);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GenerateTextureFramebufferMSAA(GLuint* texture, unsigned int width, unsigned int height, unsigned int samples)
    {
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }

    void GenerateCubemapTexture(const std::vector<std::string>& faces, GLuint* texture, GLint filteringOption)
    {
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            stbi_uc* data = nullptr;
            data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
            OPENGLCORE_ASSERT(data, "Failed to load image.");

            GLenum internalFormat, dataFormat;
            switch (channels)
            {
            case 1:
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                break;
            case 2:
                internalFormat = GL_RG8;
                dataFormat = GL_RG;
                break;
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            default:
                OPENGLCORE_ASSERT(false, "Do not support textures with more than 4 channels.");
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }


        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filteringOption);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filteringOption);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

}