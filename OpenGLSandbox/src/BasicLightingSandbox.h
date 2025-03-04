#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

#include "Lighting/LightCasters.h"
#include "Material/MaterialProperties.h"

#define NR_POINT_LIGHTS 4

class BasicLightingSandbox : public OpenGLCore::Layer
{
public:
    BasicLightingSandbox();
    virtual ~BasicLightingSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    void InitializeLights();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
private:
    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;

    // Lights
    std::unique_ptr<DirectionalLight> m_DirectionalLight;
    std::array<std::unique_ptr<PointLight>, NR_POINT_LIGHTS> m_PointLights;
    std::unique_ptr<SpotLight> m_FlashLight;

    OpenGLCore::Utils::Shader* m_PhongShader;

    GLuint m_UBOMatrices;

    GLuint m_CubeVAO;

    PhongMaterial m_Material;

    // Cube positions
    const std::array<glm::vec3, 10> c_CubePositions = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
};

