#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class WavesSandbox : public OpenGLCore::Layer
{
public:
    WavesSandbox();
    ~WavesSandbox();

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

    // TODO : create struct or class to hold these together & update in all sandboxes
    tinygltf::Model* m_Model;
    std::pair<GLuint, std::map<int, GLuint>> m_VaoAndEbos;

    OpenGLCore::Utils::Shader* m_WavesShader;

    // TODO : Add skybox as seperate layer? Would be rendered first
};

