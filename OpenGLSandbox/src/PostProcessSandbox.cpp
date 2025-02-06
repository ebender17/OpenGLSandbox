#include "PostProcessSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

PostProcessSandbox::PostProcessSandbox()
    : Layer("PostProcessSandbox")
{
}

PostProcessSandbox::~PostProcessSandbox()
{
}

void PostProcessSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // TODO : check this
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // default
    glFrontFace(GL_CW); // default is GL_CCW

    // vertex position, tex coords
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
    };
    // positions, tex coords
    float quadVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    };
    float fullscreenQuadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);
    glGenBuffers(1, &m_CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &m_QuadVAO);
    glBindVertexArray(m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &m_FullscreenVAO);
    glBindVertexArray(m_FullscreenVAO);
    glGenBuffers(1, &m_FullscreenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_FullscreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenQuadVertices), fullscreenQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_WoodenBoxTexture, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT, GL_LINEAR);

    m_TextureUnlitShader = Shader::FromGLSLTextFiles(
        "assets/shaders/textureunlit.vert.glsl",
        "assets/shaders/textureunlit.frag.glsl"
    );
    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformInt("u_Texture", 0);

    m_FullScreenQuadShader = Shader::FromGLSLTextFiles(
        "assets/shaders/postprocess.vert.glsl",
        "assets/shaders/postprocess.frag.glsl"
    );
    glUseProgram(m_FullScreenQuadShader->GetRendererID());
    m_FullScreenQuadShader->UploadUniformInt("u_ScreenTexture", 0);

    glBindVertexArray(0);

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    GenerateTextureFramebuffer(&m_FramebufferTexture, WINDOW_WIDTH, WINDOW_HEIGHT, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer is not complete!");
    }
    // bind default framebuffer (main window)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteBuffers(1, &m_CubeVBO);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteBuffers(1, &m_QuadVBO);
    glDeleteVertexArrays(1, &m_FullscreenVAO);
    glDeleteBuffers(1, &m_FullscreenVBO);
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_RBO);
}

void PostProcessSandbox::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PostProcessSandbox::OnWindowResized));
}

void PostProcessSandbox::OnUpdate(Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    // first pass
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawScene();

    // second pass
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_FullScreenQuadShader->GetRendererID());
    m_FullScreenQuadShader->UploadUniformInt("u_PostProcessOption", (int)m_PostProcessOption);
    glBindVertexArray(m_FullscreenVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessSandbox::OnImGuiRender()
{
    static int currentOption = 0;
    ImGui::Begin("Post Process Settings");
    if (ImGui::RadioButton("Inversion", currentOption == 0))
        currentOption = 0;
    if (ImGui::RadioButton("Grayscale", currentOption == 1))
        currentOption = 1;
    if (ImGui::RadioButton("Sharpen", currentOption == 2))
        currentOption = 2;
    if (ImGui::RadioButton("Blur", currentOption == 3))
        currentOption = 3;

    m_PostProcessOption = PostProcesOptions(currentOption);
    ImGui::End();
}

void PostProcessSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool PostProcessSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}

void PostProcessSandbox::DrawScene()
{
    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glm::vec3 camPos = m_Camera->GetPosition();
    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);

    // Floor
    glDisable(GL_CULL_FACE);
    glBindVertexArray(m_QuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_MetalTexture);
    glm::mat4 modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f));
    modelFloor = glm::rotate(modelFloor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelFloor = glm::scale(modelFloor, glm::vec3(7.0f, 7.0f, 1.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", modelFloor);
    m_TextureUnlitShader->UploadUniformFloat("u_TilingFactor", 7.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw boxes
    glEnable(GL_CULL_FACE);
    glBindVertexArray(m_CubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodenBoxTexture);
    m_TextureUnlitShader->UploadUniformFloat("u_TilingFactor", 1.0f);
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}
