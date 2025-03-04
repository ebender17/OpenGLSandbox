#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class PostProcessSandbox : public OpenGLCore::Layer
{
public:
    enum class PostProcesOptions
    {
        Inversion = 0,
        Grayscale,
        Sharpen,
        Blur
    };
public:
    PostProcessSandbox();
    virtual ~PostProcessSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
    void DrawScene();
private:
    GLuint m_UBOMatrices;
    GLuint m_CubeVAO;
    GLuint m_QuadVAO;
    GLuint m_FullscreenVAO;
    GLuint m_FBO;
    GLuint m_RBO;

    GLuint m_WoodenBoxTexture;
    GLuint m_MetalTexture;
    GLuint m_FramebufferTexture;

    OpenGLCore::Utils::Shader* m_TextureUnlitShader;
    OpenGLCore::Utils::Shader* m_FullScreenQuadShader;

    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;
    float m_WindowWidth;
    float m_WindowHeight;

    PostProcesOptions m_PostProcessOption = PostProcesOptions::Inversion;
};