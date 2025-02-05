#include "BasicLightingSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

BasicLightingSandbox::BasicLightingSandbox()
    : Layer("BasicLightingSandbox")
{
}

BasicLightingSandbox::~BasicLightingSandbox()
{
}

void BasicLightingSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::High);

    InitializeCamera();
    InitializeLights();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // vertex position, tex coords, normals
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    };

    // Setup buffers
    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);

    glGenBuffers(1, &m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load and create textures
    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_Material.DiffuseId, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/wooden-box-specular.png", &m_Material.SpecularId, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/box-emissive.png", &m_Material.EmissionId, GL_REPEAT, GL_LINEAR);

    m_PhongShader = Shader::FromGLSLTextFiles(
        "assets/shaders/phong.vert.glsl",
        "assets/shaders/phong.frag.glsl"
    );

    // Material uniforms
    glUseProgram(m_PhongShader->GetRendererID());
    m_PhongShader->UploadUniformInt("u_Material.diffuse", 0);
    m_PhongShader->UploadUniformInt("u_Material.specular", 1);
    m_PhongShader->UploadUniformInt("u_Material.emission", 2);
    m_PhongShader->UploadUniformFloat("u_Material.shininess", 32.f);

    // Lighting uniforms
    m_PhongShader->UploadUniformFloat3("u_DirLight.direction", m_DirectionalLight->Direction);
    m_PhongShader->UploadUniformFloat3("u_DirLight.ambient", m_DirectionalLight->Ambient);
    m_PhongShader->UploadUniformFloat3("u_DirLight.diffuse", m_DirectionalLight->Diffuse);
    m_PhongShader->UploadUniformFloat3("u_DirLight.specular", m_DirectionalLight->Specular);

    for (int i = 0; i < 4; i++)
    {
        m_PhongShader->UploadUniformFloat3(std::format("u_PointLights[{}].position", i), m_PointLights[i]->Position);
        m_PhongShader->UploadUniformFloat3(std::format("u_PointLights[{}].ambient", i), m_PointLights[i]->Ambient);
        m_PhongShader->UploadUniformFloat3(std::format("u_PointLights[{}].diffuse", i), m_PointLights[i]->Diffuse);
        m_PhongShader->UploadUniformFloat3(std::format("u_PointLights[{}].specular", i), m_PointLights[i]->Specular);
        m_PhongShader->UploadUniformFloat(std::format("u_PointLights[{}].constant", i), m_PointLights[i]->Constant);
        m_PhongShader->UploadUniformFloat(std::format("u_PointLights[{}].linear", i), m_PointLights[i]->Linear);
        m_PhongShader->UploadUniformFloat(std::format("u_PointLights[{}].quadratic", i), m_PointLights[i]->Quadratic);
    }

    m_PhongShader->UploadUniformFloat3("u_SpotLight.ambient", m_FlashLight->Ambient);
    m_PhongShader->UploadUniformFloat3("u_SpotLight.diffuse", m_FlashLight->Diffuse);
    m_PhongShader->UploadUniformFloat3("u_SpotLight.specular", m_FlashLight->Specular);
    m_PhongShader->UploadUniformFloat("u_SpotLight.constant", m_FlashLight->Constant);
    m_PhongShader->UploadUniformFloat("u_SpotLight.linear", m_FlashLight->Linear);
    m_PhongShader->UploadUniformFloat("u_SpotLight.quadratic", m_FlashLight->Quadratic);
    m_PhongShader->UploadUniformFloat("u_SpotLight.innerCutOff", m_FlashLight->InnerCutOff);
    m_PhongShader->UploadUniformFloat("u_SpotLight.outerCutOff", m_FlashLight->OuterCutOff);
}

void BasicLightingSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteVertexArrays(1, &m_VBO);
}

void BasicLightingSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(BasicLightingSandbox::OnWindowResized));
}

void BasicLightingSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glm::vec3 camPos = m_Camera->GetPosition();
    glUseProgram(m_PhongShader->GetRendererID());
    m_PhongShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
    m_PhongShader->UploadUniformFloat3("u_ViewPos", camPos);
    m_PhongShader->UploadUniformFloat("u_Time", glfwGetTime());
    // Spotlight
    m_PhongShader->UploadUniformFloat3("u_SpotLight.position", camPos);
    m_PhongShader->UploadUniformFloat3("u_SpotLight.direction", m_Camera->GetForward());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Material.DiffuseId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Material.SpecularId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Material.EmissionId);

    glBindVertexArray(m_CubeVAO);
    for (unsigned int i = 0; i < c_CubePositions.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, c_CubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_PhongShader->UploadUniformMat4("u_Model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void BasicLightingSandbox::OnImGuiRender()
{
}

void BasicLightingSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

void BasicLightingSandbox::InitializeLights()
{
    m_DirectionalLight = std::make_unique<DirectionalLight>(glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.05f, 0.05f, 0.1f),
        glm::vec3(0.2f, 0.2f, 0.7),
        glm::vec3(0.7f, 0.7f, 0.7f));

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    glm::vec3 pointLightColors[] = {
        glm::vec3(0.2f, 0.2f, 0.6f),
        glm::vec3(0.3f, 0.3f, 0.7f),
        glm::vec3(0.0f, 0.0f, 0.3f),
        glm::vec3(0.4f, 0.4f, 0.4f)
    };

    m_PointLights = {
        std::make_unique<PointLight>(glm::vec3(0.7f,  0.2f,  2.0f),
        pointLightColors[0] * glm::vec3(0.1f), pointLightColors[0], pointLightColors[0], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(2.3f, -3.3f, -4.0f),
        pointLightColors[1] * glm::vec3(0.1f), pointLightColors[1], pointLightColors[1], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(-4.0f,  2.0f, -12.0f),
        pointLightColors[2] * glm::vec3(0.1f), pointLightColors[2], pointLightColors[2], constant, linear, quadratic),
        std::make_unique<PointLight>(glm::vec3(0.0f,  0.0f, -3.0f),
        pointLightColors[3] * glm::vec3(0.1f), pointLightColors[3], pointLightColors[3], constant, linear, quadratic),
    };

    m_FlashLight = std::make_unique<SpotLight>(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f, 0.09f, 0.032f, glm::cos(glm::radians(10.0f)), glm::cos(glm::radians(15.0f)));
}

bool BasicLightingSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
