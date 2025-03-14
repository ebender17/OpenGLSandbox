#include "GeometryShaderSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

GeometryShaderSandbox::GeometryShaderSandbox()
    : Layer("GeometryShaderSandbox")
{
}

GeometryShaderSandbox::~GeometryShaderSandbox()
{
}

void GeometryShaderSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    // uniform buffers
    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // model
    m_Model = new tinygltf::Model();
    gltfModel::LoadModel(*m_Model, "assets/models/Suzanne/Suzanne.gltf");
    // TODO : it returns vbos not ebos but then we use these vbos to get the ebos when DrawModel, rename?
    m_VaoAndEbos = gltfModel::BindModel(*m_Model);
    gltfModel::PrintModelData(*m_Model);

    // shaders
    m_UnlitShader = Shader::FromGLSLTextFiles(
        "assets/shaders/gltf.vert.glsl",
        "assets/shaders/gltf.frag.glsl"
    );

    m_ExplodeNormalsShader = Shader::FromGLSLTextFiles(
        "assets/shaders/geometry-shaders/explode-normals.vert.glsl",
        "assets/shaders/geometry-shaders/explode-normals.geom.glsl",
        "assets/shaders/geometry-shaders/explode-normals.frag.glsl"
    );

    m_DisplayNormalsShader = Shader::FromGLSLTextFiles(
        "assets/shaders/geometry-shaders/display-normals.vert.glsl",
        "assets/shaders/geometry-shaders/display-normals.geom.glsl",
        "assets/shaders/geometry-shaders/display-normals.frag.glsl"
    );
    glUseProgram(m_DisplayNormalsShader->GetRendererID());
    m_DisplayNormalsShader->UploadUniformFloat3("u_Color", { 1.0f, 0.0f, 0.0f });
    glLineWidth(1.0f);
}

void GeometryShaderSandbox::OnDetach()
{
    delete m_UnlitShader;
    delete m_ExplodeNormalsShader;
    delete m_DisplayNormalsShader;
}

void GeometryShaderSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(GeometryShaderSandbox::OnWindowResized));
}

void GeometryShaderSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
    glm::mat4 projMatrix = m_Camera->GetProjMatrix();
    glm::mat4 viewProjectionMatrix = projMatrix * viewMatrix;
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glUseProgram(m_ExplodeNormalsShader->GetRendererID());
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-5.0f, 0.0f, -1.0f));
    m_ExplodeNormalsShader->UploadUniformMat4("u_Model", modelMatrix);
    m_ExplodeNormalsShader->UploadUniformFloat("u_Time", static_cast<float>(glfwGetTime()));
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);

    glUseProgram(m_UnlitShader->GetRendererID());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(5.0f, 0.0f, -1.0f));
    m_UnlitShader->UploadUniformMat4("u_Model", modelMatrix);
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);

    glUseProgram(m_DisplayNormalsShader->GetRendererID());
    m_DisplayNormalsShader->UploadUniformMat4("u_Model", modelMatrix);
    m_DisplayNormalsShader->UploadUniformMat4("u_View", viewMatrix);
    m_DisplayNormalsShader->UploadUniformMat4("u_Projection", projMatrix);
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);
}

void GeometryShaderSandbox::OnImGuiRender()
{
}

void GeometryShaderSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool GeometryShaderSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
