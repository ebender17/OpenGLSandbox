#include "HDRandBloomSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

HDRandBloomSandbox::HDRandBloomSandbox()
    : Layer("HDRandBloomSandbox")
{
}

HDRandBloomSandbox::~HDRandBloomSandbox()
{
}

void HDRandBloomSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    CreateCubeVAO();
    CreateQuadVAO();

    // create floating point framebuffer
    glGenFramebuffers(1, &m_hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    // create floating point color buffer
    glGenTextures(2, m_ColorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorBuffers[i], 0);
    }
    // create depth buffer and attach
    glGenRenderbuffers(1, &m_DepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring
    glGenFramebuffers(2, m_PingPongFBOs);
    glGenTextures(2, m_PingPongColorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBOs[i]);
        glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingPongColorBuffers[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("Framebuffer is not complete!");
        }
    }

    // create shaders
    m_LightingShader = Shader::FromGLSLTextFiles(
        "assets/shaders/hdr/hdr-lighting.vert.glsl",
        "assets/shaders/hdr/hdr-lighting-with-bloom.frag.glsl"
    );
    m_LightShader = Shader::FromGLSLTextFiles(
        "assets/shaders/hdr/hdr-lighting.vert.glsl",
        "assets/shaders/hdr/hdr-light-with-bloom.frag.glsl"
    );
    m_BlurShader = Shader::FromGLSLTextFiles(
        "assets/shaders/fullscreen-quad.vert.glsl",
        "assets/shaders/blur/gaussian-blur.frag.glsl"
    );
    m_HDRShader = Shader::FromGLSLTextFiles(
        "assets/shaders/fullscreen-quad.vert.glsl",
        "assets/shaders/hdr/hdr-with-bloom.frag.glsl"
    );
    glUseProgram(m_LightingShader->GetRendererID());
    m_LightingShader->UploadUniformInt("u_DiffuseTexture", 0);
    glUseProgram(m_BlurShader->GetRendererID());
    m_BlurShader->UploadUniformInt("u_Image", 0);
    m_BlurShader->UploadUniformFloatArray("u_Weights", m_GaussianWeights.data(), 5);
    glUseProgram(m_HDRShader->GetRendererID());
    m_HDRShader->UploadUniformInt("u_HDRBuffer", 0);
    m_HDRShader->UploadUniformInt("u_BloomBlurBuffer", 1);

    // generate sRGB diffuse texture
    GenerateTexture2D("assets/textures/wood.png", &m_WoodDiffuse, GL_REPEAT, GL_LINEAR, true);
}

void HDRandBloomSandbox::OnDetach()
{
    glDeleteFramebuffers(1, &m_hdrFBO);
    glDeleteFramebuffers(2, m_PingPongFBOs);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteVertexArrays(1, &m_CubeVAO);
    delete m_LightingShader;
    delete m_HDRShader;
}

void HDRandBloomSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(HDRandBloomSandbox::OnWindowResized));
}

void HDRandBloomSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    // make sure clear color is black so it doesn't leak into framebuffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // render scene into floating point framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // lighting
    glUseProgram(m_LightingShader->GetRendererID());
    m_LightingShader->UploadUniformFloat3("u_ViewPos", m_Camera->GetPosition());
    for (int i = 0; i < m_LightPositions.size(); i++)
    {
        m_LightingShader->UploadUniformFloat3("u_Lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
        m_LightingShader->UploadUniformFloat3("u_Lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
    }
    // tunnel
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodDiffuse);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
    model = glm::scale(model, glm::vec3(5.0f, 2.5f, 27.5f));
    m_LightingShader->UploadUniformMat4("u_Model", model);
    m_LightingShader->UploadUniformInt("u_InverseNormals", true);
    glBindVertexArray(m_CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // light cubes
    glUseProgram(m_LightShader->GetRendererID());
    for (unsigned int i = 0; i < m_LightColors.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_LightPositions[i]));
        model = glm::scale(model, glm::vec3(0.25f));
        m_LightShader->UploadUniformMat4("u_Model", model);
        m_LightShader->UploadUniformFloat3("u_LightColor", m_LightColors[i]);
        m_LightShader->UploadUniformInt("u_InverseNormals", false);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // blur bright fragments with two-pass Gaussian Blur
    glBindVertexArray(m_QuadVAO);
    bool horizontal = true, first_iteration = true;
    glUseProgram(m_BlurShader->GetRendererID());
    for (unsigned int i = 0; i < m_BlurAmount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBOs[horizontal]);
        m_BlurShader->UploadUniformInt("u_Horizontal", horizontal);
        glActiveTexture(GL_TEXTURE0);
        // on the first iteration need to get the bloom color
        glBindTexture(GL_TEXTURE_2D, first_iteration ? m_ColorBuffers[1] : m_PingPongColorBuffers[!horizontal]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        horizontal = !horizontal;
        if (first_iteration)
        {
            first_iteration = false;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // render floating point buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_HDRShader->GetRendererID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    // last pass was on vertical pass so retrieve that one
    glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[!horizontal]);
    m_HDRShader->UploadUniformInt("u_Bloom", m_EnableBloom);
    m_HDRShader->UploadUniformInt("u_HDR", m_EnableHDR);
    m_HDRShader->UploadUniformFloat("u_Exposure", m_Exposure);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void HDRandBloomSandbox::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Checkbox("Enable HDR", &m_EnableHDR);
    ImGui::SliderFloat("Exposure", &m_Exposure, c_ExposureMin, c_ExposureMax);
    ImGui::Checkbox("Enable Bloom", &m_EnableBloom);
    ImGui::SliderInt("Blur Amount", &m_BlurAmount, c_BlurAmountMin, c_BlurAmountMax);
    ImGui::End();
}

void HDRandBloomSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool HDRandBloomSandbox::OnWindowResized(WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
        // reallocate with new dimensions (NULL leaves existing data undefined)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBOs[i]);
        glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return false;
}

void HDRandBloomSandbox::CreateCubeVAO()
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         // bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         // top face
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
          1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
    };
    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HDRandBloomSandbox::CreateQuadVAO()
{
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &m_QuadVAO);
    glBindVertexArray(m_QuadVAO);
    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
