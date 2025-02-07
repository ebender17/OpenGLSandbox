#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class PostProcessMSAASandbox : public OpenGLCore::Layer
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
    PostProcessMSAASandbox();
    virtual ~PostProcessMSAASandbox();

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
    GLuint m_CubeVAO;
    GLuint m_CubeVBO;
    GLuint m_QuadVAO;
    GLuint m_QuadVBO;
    GLuint m_FullscreenVAO;
    GLuint m_FullscreenVBO;

    GLuint m_FBO;
    GLuint m_RBO;
    GLuint m_IntermediateFBO;

    GLuint m_WoodenBoxTexture;
    GLuint m_MetalTexture;
    GLuint m_TextureColorBufferMultiSampled;
    GLuint m_ScreenTexture;

    OpenGLCore::Utils::Shader* m_TextureUnlitShader;
    OpenGLCore::Utils::Shader* m_FullScreenQuadShader;

    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;
    float m_WindowWidth;
    float m_WindowHeight;

    PostProcesOptions m_PostProcessOption = PostProcesOptions::Inversion;
};

