#include "NormalandParallaxMappingSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

NormalandParallaxMappingSandbox::NormalandParallaxMappingSandbox()
    : Layer("NormalandParallaxMappingSandbox")
{
}

NormalandParallaxMappingSandbox::~NormalandParallaxMappingSandbox()
{
}

void NormalandParallaxMappingSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    // positions
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3(1.0f, -1.0f, 0.0f);
    glm::vec3 pos4(1.0f, 1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1 - 1, 2, 3
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;
    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    // triangle 2 - 1, 3, 4
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;
    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

    GLuint planeVBO;
    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &planeVBO);

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GenerateTexture2D("assets/textures/brickwall/brickwall-diffuse.jpg", &m_BrickwallDiffuse, GL_REPEAT, GL_LINEAR, true);
    GenerateTexture2D("assets/textures/brickwall/brickwall-normal.jpg", &m_BrickwallNormal, GL_REPEAT, GL_LINEAR, false);
    GenerateTexture2D("assets/textures/brickwall/red-brickwall-diffuse.jpg", &m_RedBrickwallDiffuse, GL_REPEAT, GL_LINEAR, true);
    GenerateTexture2D("assets/textures/brickwall/red-brickwall-normal.jpg", &m_RedBrickwallNormal, GL_REPEAT, GL_LINEAR, false);
    GenerateTexture2D("assets/textures/brickwall/red-brickwall-disp.jpg", &m_RedBrickwallDisplacement, GL_REPEAT, GL_LINEAR, false);

    m_NormalMappingShader = Shader::FromGLSLTextFiles(
        "assets/shaders/normal-mapping/normal-mapping.vert.glsl",
        "assets/shaders/normal-mapping/normal-mapping.frag.glsl"
    );
    glUseProgram(m_NormalMappingShader->GetRendererID());
    m_NormalMappingShader->UploadUniformInt("u_DiffuseMap", 0);
    m_NormalMappingShader->UploadUniformInt("u_NormalMap", 1);
    m_NormalMappingShader->UploadUniformFloat3("u_LightPos", m_LightPos);

    m_ParallaxMappingShader = Shader::FromGLSLTextFiles(
        "assets/shaders/parallax-mapping/parallax-mapping.vert.glsl",
        "assets/shaders/parallax-mapping/parallax-mapping.frag.glsl"
    );
    glUseProgram(m_ParallaxMappingShader->GetRendererID());
    m_ParallaxMappingShader->UploadUniformInt("u_DiffuseMap", 0);
    m_ParallaxMappingShader->UploadUniformInt("u_NormalMap", 1);
    m_ParallaxMappingShader->UploadUniformInt("u_DisplacementMap", 2);
    m_ParallaxMappingShader->UploadUniformFloat3("u_Light", m_LightPos);
}

void NormalandParallaxMappingSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_QuadVAO);
    delete m_NormalMappingShader;
    delete m_ParallaxMappingShader;
}

void NormalandParallaxMappingSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(NormalandParallaxMappingSandbox::OnWindowResized));
}

void NormalandParallaxMappingSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glDisable(GL_FRAMEBUFFER_SRGB);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindVertexArray(m_QuadVAO);
    glUseProgram(m_NormalMappingShader->GetRendererID());
    m_NormalMappingShader->UploadUniformFloat3("u_ViewPos", m_Camera->GetPosition());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0, 0.0, 0.0));
    m_NormalMappingShader->UploadUniformMat4("u_Model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_BrickwallDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_BrickwallNormal);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(m_ParallaxMappingShader->GetRendererID());
    m_ParallaxMappingShader->UploadUniformFloat3("u_ViewPos", m_Camera->GetPosition());
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0, 0.0, 0.0));
    m_ParallaxMappingShader->UploadUniformMat4("u_Model", model);
    m_ParallaxMappingShader->UploadUniformFloat("u_HeightScale", m_HeightScale);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_RedBrickwallDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_RedBrickwallNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_RedBrickwallDisplacement);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void NormalandParallaxMappingSandbox::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::SliderFloat("Height Scale", &m_HeightScale, c_HeightMin, c_HeightMax);
    ImGui::End();
}

void NormalandParallaxMappingSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool NormalandParallaxMappingSandbox::OnWindowResized(WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}