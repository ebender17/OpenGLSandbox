#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class InstancingSandbox : public OpenGLCore::Layer
{
public:
    InstancingSandbox();
    virtual ~InstancingSandbox();

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
    GLuint m_CubeVAO;
    OpenGLCore::Utils::Shader* m_InstancedShader;

    const int c_GridSize = 10;
    const int c_NumberOfCubes = c_GridSize * c_GridSize;
};