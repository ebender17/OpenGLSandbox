#include "BlinnPhongSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

BlinnPhongSandbox::BlinnPhongSandbox()
    : Layer("BlinnPhongSandbox")
{
}

BlinnPhongSandbox::~BlinnPhongSandbox()
{
}

void BlinnPhongSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_MULTISAMPLE);

    float planeVertices[] = {
        // positions            // normals         // texcoords
         5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  5.0f,  0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 5.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,  5.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f, 5.0f,
         5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,  5.0f, 5.0f
    };

    GLuint planeVBO;
    glGenVertexArrays(1, &m_PlaneVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(m_PlaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &planeVBO);


    GenerateTexture2D("assets/textures/wood.png", &m_WoodTexture, GL_REPEAT, GL_LINEAR);

    m_PhongBlinnPhongShader = Shader::FromGLSLTextFiles(
        "assets/shaders/blinnphong.vert.glsl",
        "assets/shaders/blinnphong.frag.glsl"
    );

    glUseProgram(m_PhongBlinnPhongShader->GetRendererID());
    m_PhongBlinnPhongShader->UploadUniformInt("u_Texture", 0);
}

void BlinnPhongSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_PlaneVAO);

    delete m_PhongBlinnPhongShader;
}

void BlinnPhongSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(BlinnPhongSandbox::OnWindowResized));
}

void BlinnPhongSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glUseProgram(m_PhongBlinnPhongShader->GetRendererID());
    m_PhongBlinnPhongShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);

    glBindVertexArray(m_PlaneVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodTexture);
    m_PhongBlinnPhongShader->UploadUniformFloat3("u_ViewPos", m_Camera->GetPosition());
    m_PhongBlinnPhongShader->UploadUniformFloat3("u_LightPos", m_LightPos);
    m_PhongBlinnPhongShader->UploadUniformInt("u_Blinn", m_BlinnPhong);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void BlinnPhongSandbox::OnImGuiRender()
{
    static bool isChecked = true;

    ImGui::Begin("Lighting Model Settings");
    if (ImGui::Checkbox("Enable Blinn Phong? Else use Phong.", &isChecked))
    {
        m_BlinnPhong = isChecked;
    }

    ImGui::End();
}

void BlinnPhongSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool BlinnPhongSandbox::OnWindowResized(WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}