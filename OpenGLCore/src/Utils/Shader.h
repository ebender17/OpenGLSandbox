#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace OpenGLCore::Utils {

    class Shader
    {
    public:
        ~Shader();

        GLuint GetRendererID() { return m_RendererID; }
        static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        // Uniforms
        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat3Array(const std::string& name, float* values, uint32_t count);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        Shader() = default;

        void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static std::string ReadFileAsString(const std::string& filepath);
        GLuint CompileShader(GLenum type, const std::string& source);
        unsigned int GetUniformLocation(const std::string& name);
    private:
        GLuint m_RendererID;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    };

}