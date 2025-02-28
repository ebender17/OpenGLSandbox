#pragma once

#include <string>

#include <glad/glad.h>

namespace OpenGLCore::Utils {

    void GenerateTexture2D(const std::string& filepath, GLuint* texture, GLint wrapOption, GLint filteringOption);

    void GenerateTextureFramebuffer(GLuint* texture, unsigned int width, unsigned int height, GLint filteringOption);

    void GenerateTextureFramebufferMSAA(GLuint* texture, unsigned int width, unsigned int height, unsigned int samples);

    void GenerateCubemapTexture(const std::vector<std::string>& faces, GLuint* texture, GLint filteringOption);

}

