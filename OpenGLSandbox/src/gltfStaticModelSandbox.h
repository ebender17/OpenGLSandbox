#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

#include <tiny_gltf.h>

class gltfStaticModelSandbox : public OpenGLCore::Layer
{
public:
    gltfStaticModelSandbox();
    virtual ~gltfStaticModelSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
private:
    tinygltf::Model m_Model;
};

