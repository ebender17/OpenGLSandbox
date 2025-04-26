#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class NormalandParallaxMappingSandbox : public OpenGLCore::Layer
{
public:
    NormalandParallaxMappingSandbox();
    virtual ~NormalandParallaxMappingSandbox();

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
    OpenGLCore::Utils::Shader* m_ParallaxMappingShader;

    GLuint m_BrickwallDiffuse;
    GLuint m_BrickwallNormal;

    GLuint m_RedBrickwallDiffuse;
    GLuint m_RedBrickwallNormal;
    GLuint m_RedBrickwallDisplacement;

    float m_HeightScale = 0.1f;
    const float c_HeightMin = 0.01f;
    const float c_HeightMax = 0.1f;

    glm::vec3 m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
};

