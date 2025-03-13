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

    GLuint m_UBOMatrices;

    GLuint m_PlaneVAO;

    OpenGLCore::Utils::Shader* m_PhongBlinnPhongShader;
    GLuint m_WoodTexture;

    bool m_BlinnPhong = true;
    glm::vec3 m_LightPositions[4] = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(3.0f, 0.0f, 0.0f)
    };
    glm::vec3 m_LightColors[4] = {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
    };
};

