#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

#include "Material/MaterialProperties.h"

class CubemapSandbox : public OpenGLCore::Layer
{
public:
    CubemapSandbox();
    ~CubemapSandbox();

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

    GLuint m_SkyboxVAO;
    OpenGLCore::Utils::Shader* m_SkyboxShader;
    std::vector<std::string> m_Faces
    {
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_right.png",
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_left.png",
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_up.png",
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_down.png",
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_front.png",
        "assets/textures/cubemaps/ClearOcean/vz_clear_ocean_back.png"
    };
    GLuint m_SkyboxTexture;

    // TODO : create struct or class to hold these together
    tinygltf::Model* m_Model;
    std::pair<GLuint, std::map<int, GLuint>> m_VaoAndEbos;

    OpenGLCore::Utils::Shader* m_ReflectionShader;
    OpenGLCore::Utils::Shader* m_RefractionShader;
    float m_FromRefractiveIndex = RefractiveIndex::Air;
    float m_ToRefractiveIndex = RefractiveIndex::Water;
};

