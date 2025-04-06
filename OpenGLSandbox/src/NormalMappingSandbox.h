#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class NormalMappingSandbox : public OpenGLCore::Layer
{
public:
    NormalMappingSandbox();
    virtual ~NormalMappingSandbox();

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

    GLuint m_UBOMatrices;

    GLuint m_QuadVAO;

    OpenGLCore::Utils::Shader* m_NormalMappingShader;
    GLuint m_BrickwallDiffuse;
    GLuint m_BrickwallNormal;

    glm::vec3 m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
};

