#include "WavesSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

WavesSandbox::WavesSandbox()
    : Layer("WavesSandbox")
{
}

WavesSandbox::~WavesSandbox()
{
}

void WavesSandbox::OnAttach()
{
    // TODO : use these debug levels
    // TODO : Use tagging with RenderDoc to debug as well
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::HighAssert);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // TODO : enable again
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    glEnable(GL_MULTISAMPLE);

    m_Model = new tinygltf::Model();
    gltfModel::LoadModel(*m_Model, "assets/models/TesselatedPlane/tesselated-plane.gltf");
    // TODO : it returns vbos not ebos but then we use these vbos to get the ebos when DrawModel, rename?
    m_VaoAndEbos = gltfModel::BindModel(*m_Model);
    // TODO : debug
    gltfModel::PrintModelData(*m_Model);

    m_WavesShader = Shader::FromGLSLTextFiles(
        "assets/shaders/waves/waves.vert.glsl",
        "assets/shaders/waves/waves.frag.glsl"
    );

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void WavesSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_VaoAndEbos.first);
    delete m_Model;
    delete m_WavesShader;
}

void WavesSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(WavesSandbox::OnWindowResized));
}

void WavesSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    // Clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO : remove after done debugging grid
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Time
    m_WavesShader->UploadUniformFloat("u_Time", static_cast<float>(glfwGetTime()));


    // ViewProjection Matrix
    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Model Matrix
    glUseProgram(m_WavesShader->GetRendererID());
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_WavesShader->UploadUniformMat4("u_Model", model1);

    // TODO : how to activate textures
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_TileTexture);

    // Draw
    gltfModel::DrawModel(m_VaoAndEbos, *m_Model);
}

void WavesSandbox::OnImGuiRender()
{
    // TODO
}

void WavesSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool WavesSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
