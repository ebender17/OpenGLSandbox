#include "glpch.h"
#include "Texture.h"

#include <stb_image.h>

#include "Core/Core.h"

namespace OpenGLCore::Utils {

    void GenerateTexture2D(const std::string& filepath, GLuint* texture, GLint wrapOption, GLint filteringOption)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);

        stbi_uc* data = nullptr;
        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        OPENGLCORE_ASSERT(data, "Failed to load image.");

        GLenum internalFormat, dataFormat;
        switch (channels)
        {
        case 1:
            OPENGLCORE_ASSERT(false, "Do not yet support textures with 1 channel.");
            break;
        case 2:
            OPENGLCORE_ASSERT(false, "Do not yet support textures with 2 channels.");
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

}