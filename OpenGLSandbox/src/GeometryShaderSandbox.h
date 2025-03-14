#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class GeometryShaderSandbox : public OpenGLCore::Layer
{
public:
    GeometryShaderSandbox();
    virtual ~GeometryShaderSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
private:
    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;

    // TODO : create struct or class to hold these together
    tinygltf::Model* m_Model;
    std::pair<GLuint, std::map<int, GLuint>> m_VaoAndEbos;

    GLuint m_UBOMatrices;

    OpenGLCore::Utils::Shader* m_ExplodeNormalsShader;
    OpenGLCore::Utils::Shader* m_UnlitShader;
    OpenGLCore::Utils::Shader* m_DisplayNormalsShader;
};