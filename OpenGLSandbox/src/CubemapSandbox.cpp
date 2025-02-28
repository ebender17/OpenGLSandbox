#include "CubemapSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

CubemapSandbox::CubemapSandbox()
    : Layer("CubemapSandbox")
{
}

CubemapSandbox::~CubemapSandbox()
{
}

void CubemapSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::HighAssert);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // TODO : enable again
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    glEnable(GL_MULTISAMPLE);

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_SkyboxVAO);
    GLuint skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(m_SkyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &skyboxVBO);

    m_SkyboxShader = Shader::FromGLSLTextFiles(
        "assets/shaders/skybox/skybox.vert.glsl",
        "assets/shaders/skybox/skybox.frag.glsl"
    );
    glUseProgram(m_SkyboxShader->GetRendererID());
    m_SkyboxShader->UploadUniformInt("u_Texture", 0);
    GenerateCubemapTexture(m_Faces, &m_SkyboxTexture, GL_LINEAR);

    m_Model = new tinygltf::Model();
    gltfModel::LoadModel(*m_Model, "assets/models/Suzanne/Suzanne.gltf");
    // TODO : it returns vbos not ebos but then we use these vbos to get the ebos when DrawModel, rename?
    m_VaoAndEbos = gltfModel::BindModel(*m_Model);
    gltfModel::PrintModelData(*m_Model);

    m_ReflectionShader = Shader::FromGLSLTextFiles(
        "assets/shaders/env-mapping/reflection.vert.glsl",
        "assets/shaders/env-mapping/reflection.frag.glsl"
    );
    glUseProgram(m_ReflectionShader->GetRendererID());
    m_ReflectionShader->UploadUniformInt("u_Skybox", 0);

    m_RefractionShader = Shader::FromGLSLTextFiles(
        "assets/shaders/env-mapping/refraction.vert.glsl",
        "assets/shaders/env-mapping/refraction.frag.glsl"
    );
    glUseProgram(m_RefractionShader->GetRendererID());
    m_RefractionShader->UploadUniformInt("u_Skybox", 0);
}

void CubemapSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_SkyboxVAO);
    glDeleteVertexArrays(1, &m_VaoAndEbos.first);
    delete m_Model;
    delete m_SkyboxShader;
    delete m_ReflectionShader;
    delete m_RefractionShader;
}

void CubemapSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CubemapSandbox::OnWindowResized));
}

void CubemapSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_Camera->GetViewMatrix();
    glm::mat4 proj = m_Camera->GetProjMatrix();
    glm::mat4 viewProjectionMatrix = proj * view;

    // Draw models
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture);
    glUseProgram(m_ReflectionShader->GetRendererID());
    m_ReflectionShader->UploadUniformFloat3("u_CameraPos", m_Camera->GetPosition());
    m_ReflectionShader->UploadUniformMat4("u_Model", glm::mat4(1.0f));
    m_ReflectionShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);

    glUseProgram(m_RefractionShader->GetRendererID());
    m_RefractionShader->UploadUniformFloat("u_FromRefractionIndex", m_FromRefractiveIndex);
    m_RefractionShader->UploadUniformFloat("u_ToRefractionIndex", m_ToRefractiveIndex);
    m_RefractionShader->UploadUniformFloat3("u_CameraPos", m_Camera->GetPosition());
    m_RefractionShader->UploadUniformMat4("u_Model", glm::translate(glm::mat4(1.0f), { 4.0, 0.0, 0.0 }));
    m_RefractionShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);

    // Draw skybox last
    glDepthFunc(GL_LEQUAL);
    glm::mat4 viewProjectionMatrixNoTranslation = proj * glm::mat4(glm::mat3(view));
    glUseProgram(m_SkyboxShader->GetRendererID());
    m_SkyboxShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrixNoTranslation);
    glBindVertexArray(m_SkyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void CubemapSandbox::OnImGuiRender()
{
    static int selectedItemFrom = 0;
    static int selectedItemTo = 1;

    if (ImGui::Begin("Refractive Settings"))
    {
        ImGui::Text("Refractive Indices:");

        if (ImGui::Combo("From Index", &selectedItemFrom, RefractiveIndex::Indices, IM_ARRAYSIZE(RefractiveIndex::Indices)))
        {
            m_FromRefractiveIndex = RefractiveIndex::GetRefractiveIndex(RefractiveIndex::Indices[selectedItemFrom]);
        }

        if (ImGui::Combo("To Index", &selectedItemTo, RefractiveIndex::Indices, IM_ARRAYSIZE(RefractiveIndex::Indices)))
        {
            m_ToRefractiveIndex = RefractiveIndex::GetRefractiveIndex(RefractiveIndex::Indices[selectedItemTo]);
        }

        ImGui::End();
    }
}

void CubemapSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool CubemapSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
