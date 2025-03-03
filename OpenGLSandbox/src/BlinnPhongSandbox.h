#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class BlinnPhongSandbox : public OpenGLCore::Layer
{
public:
    BlinnPhongSandbox();
    virtual ~BlinnPhongSandbox();

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

    GLuint m_PlaneVAO;

    OpenGLCore::Utils::Shader* m_PhongBlinnPhongShader;
    GLuint m_WoodTexture;

    glm::vec3 m_LightPos = { 0.0f, 0.0f, 0.0f };

    bool m_BlinnPhong = true;
};

