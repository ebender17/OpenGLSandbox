#pragma once

#include <string>

#include <glad/glad.h>

namespace OpenGLCore::Utils {

    void GenerateTexture2D(const std::string& filepath, GLuint* texture, GLint wrapOption, GLint filteringOption);

    void GenerateTextureFramebuffer(GLuint* texture, unsigned int width, unsigned int height, GLint filteringOption);

}

