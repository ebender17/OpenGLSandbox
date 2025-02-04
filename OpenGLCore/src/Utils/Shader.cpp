#include "glpch.h"
#include "Shader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Log.h"

namespace OpenGLCore::Utils {

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    unsigned int Shader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            LOG_ERROR("Warning: uniform {0} does not exist!", name);

        m_UniformLocationCache[name] = location;
        return location;
    }

    Shader* Shader::FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        Shader* shader = new Shader();
        shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
        return shader;
    }

    void Shader::LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string vertexSource = ReadFileAsString(vertexShaderPath);
        std::string fragmentSource = ReadFileAsString(fragmentShaderPath);

        GLuint program = glCreateProgram();
        int glShaderIDIndex = 0;

        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        glAttachShader(program, vertexShader);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            LOG_ERROR("{0}", infoLog.data());
            return;
        }

        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        m_RendererID = program;
    }

    std::string Shader::ReadFileAsString(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LOG_ERROR("Could not open file '{0}'", filepath);
        }
        return result;
    }

    GLuint Shader::CompileShader(GLenum type, const std::string& source)
    {
        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);
            LOG_ERROR("{0}", infoLog.data());
        }
        return shader;
    }

    void Shader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void Shader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

}