#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class StencilSandbox : public OpenGLCore::Layer
{
public:
    StencilSandbox();
    virtual ~StencilSandbox();

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

    OpenGLCore::Utils::Shader* m_TextureUnlitShader;
    OpenGLCore::Utils::Shader* m_FlatColorShader;

    GLuint m_CubeVAO;
    GLuint m_QuadVAO;

    GLuint m_TileTexture;
    GLuint m_MetalTexture;
};

