#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class DirectionalShadowMappingSandbox : public OpenGLCore::Layer
{
public:
    DirectionalShadowMappingSandbox();
    virtual ~DirectionalShadowMappingSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
    void ConfigureLightSpaceMatrix();
    void RenderScene(OpenGLCore::Utils::Shader& shader);
private:
    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;

    glm::vec3 m_LightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
    glm::mat4 m_LightSpaceMatrix;

    OpenGLCore::Utils::Shader* m_DepthMapShader;
    OpenGLCore::Utils::Shader* m_DebugDepthMapShader;
    OpenGLCore::Utils::Shader* m_ShadowMappingShader;

    GLuint m_UBOMatrices;

    GLuint m_CubeVAO;
    GLuint m_PlaneVAO;
    GLuint m_QuadVAO;
    GLuint m_DepthMapFBO;
    GLuint m_DepthMapTexture;

    const unsigned int c_ShadowMapWidth = 1024, c_ShadowMapHeight = 1024;

    GLuint m_WoodenCrateTexture;
    GLuint m_MetalTexture;
};

