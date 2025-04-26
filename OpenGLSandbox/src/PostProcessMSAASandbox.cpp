#include "PostProcessMSAASandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

PostProcessMSAASandbox::PostProcessMSAASandbox()
    : Layer("PostProcessMSAASandbox")
{
}

PostProcessMSAASandbox::~PostProcessMSAASandbox()
{
}

void PostProcessMSAASandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

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

    GLuint cubeVBO;
    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &cubeVBO);

    GLuint quadVBO;
    glGenVertexArrays(1, &m_QuadVAO);
    glBindVertexArray(m_QuadVAO);
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &quadVBO);

    GLuint fullscreenVBO;
    glGenVertexArrays(1, &m_FullscreenVAO);
    glBindVertexArray(m_FullscreenVAO);
    glGenBuffers(1, &fullscreenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, fullscreenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenQuadVertices), fullscreenQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
    glDeleteBuffers(1, &fullscreenVBO);

    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_WoodenBoxTexture, GL_REPEAT, GL_LINEAR, false);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT, GL_LINEAR, false);

    m_TextureUnlitShader = Shader::FromGLSLTextFiles(
        "assets/shaders/textureunlit.vert.glsl",
        "assets/shaders/textureunlit.frag.glsl"
    );
    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformInt("u_Texture", 0);

    m_FullScreenQuadShader = Shader::FromGLSLTextFiles(
        "assets/shaders/fullscreen-quad.vert.glsl",
        "assets/shaders/postprocess.frag.glsl"
    );
    glUseProgram(m_FullScreenQuadShader->GetRendererID());
    m_FullScreenQuadShader->UploadUniformInt("u_ScreenTexture", 0);

    // configure MSAA framebuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    // create a multisampled color attachment texture
    GenerateTextureFramebufferMSAA(&m_TextureColorBufferMultiSampled, WINDOW_WIDTH, WINDOW_HEIGHT, MSAA_SAMPLES);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureColorBufferMultiSampled, 0);
    // also create a multisampled renderbuffer object for depth and stencil attachments
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("MSAA framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure second post-processing framebuffer
    glGenFramebuffers(1, &m_IntermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_IntermediateFBO);
    GenerateTextureFramebuffer(&m_ScreenTexture, WINDOW_WIDTH, WINDOW_HEIGHT, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture, 0); // we only need a color buffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Intermediate framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // bind default framebuffer (main window)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void PostProcessMSAASandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteVertexArrays(1, &m_FullscreenVAO);

    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_RBO);
    glDeleteFramebuffers(1, &m_IntermediateFBO);

    delete m_TextureUnlitShader;
    delete m_FullScreenQuadShader;
}

void PostProcessMSAASandbox::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(PostProcessMSAASandbox::OnWindowResized));
}

void PostProcessMSAASandbox::OnUpdate(Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    // first pass
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    DrawScene();

    // blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermediateFBO);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // draw scene as normal in multisampled buffers
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_FullScreenQuadShader->GetRendererID());
    m_FullScreenQuadShader->UploadUniformInt("u_PostProcessOption", (int)m_PostProcessOption);
    glBindVertexArray(m_FullscreenVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessMSAASandbox::OnImGuiRender()
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

void PostProcessMSAASandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool PostProcessMSAASandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}

void PostProcessMSAASandbox::DrawScene()
{
    // Floor
    glUseProgram(m_TextureUnlitShader->GetRendererID());
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
