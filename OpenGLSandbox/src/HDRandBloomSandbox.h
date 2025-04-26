#pragma once

#include <OpenGLCore.h>
#include <OpenGLCoreUtils.h>

class HDRandBloomSandbox : public OpenGLCore::Layer
{
public:
    HDRandBloomSandbox();
    virtual ~HDRandBloomSandbox();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(OpenGLCore::Event& event) override;
    virtual void OnUpdate(OpenGLCore::Timestep ts) override;
    virtual void OnImGuiRender() override;
private:
    void InitializeCamera();
    bool OnWindowResized(OpenGLCore::WindowResizeEvent& event);
    void CreateCubeVAO();
    void CreateQuadVAO();
private:
    std::unique_ptr<OpenGLCore::Utils::FirstPersonCamera> m_Camera;

    GLuint m_UBOMatrices;

    GLuint m_hdrFBO;
    GLuint m_ColorBuffers[2];
    GLuint m_DepthRBO;
    GLuint m_PingPongFBOs[2];
    GLuint m_PingPongColorBuffers[2];
    GLuint m_QuadVAO;
    GLuint m_CubeVAO;

    OpenGLCore::Utils::Shader* m_LightingShader;
    OpenGLCore::Utils::Shader* m_LightShader;
    OpenGLCore::Utils::Shader* m_BlurShader;
    OpenGLCore::Utils::Shader* m_HDRShader;

    GLuint m_WoodDiffuse;

    std::array<glm::vec3, 4> m_LightPositions = {
        glm::vec3(0.0f, 0.0f, 49.5f),
        glm::vec3(-3.5f, -0.5f, 9.0f),
        glm::vec3(0.0f, -0.8f, 15.0f),
        glm::vec3(0.5f, -1.0f, 6.0f)
    };

    std::array<glm::vec3, 4> m_LightColors = {
        glm::vec3(5.0f, 5.0f, 5.0f),
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 5.0f, 0.0f)
    };

    std::array<float, 5> m_GaussianWeights = {
        0.2270270270f,
        0.1945945946f,
        0.1216216216f,
        0.0540540541f,
        0.0162162162f,
    };

    float m_Exposure = 1.0f;
    const float c_ExposureMin = 0.1f;
    const float c_ExposureMax = 5.0f;
    bool m_EnableHDR = true;
    bool m_EnableBloom = true;
    int m_BlurAmount = 10;
    const int c_BlurAmountMin = 2;
    const int c_BlurAmountMax = 40;
};
