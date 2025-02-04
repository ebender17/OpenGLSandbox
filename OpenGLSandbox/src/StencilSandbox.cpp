#include "StencilSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

StencilSandbox::StencilSandbox()
{
}

StencilSandbox::~StencilSandbox()
{
}

void StencilSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // vertex position, tex coords
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

    // Setup buffers
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

    m_FlatColorShader = Shader::FromGLSLTextFiles(
        "assets/shaders/flatcolor.vert.glsl",
        "assets/shaders/flatcolor.frag.glsl"
    );
    glUseProgram(m_FlatColorShader->GetRendererID());
    m_FlatColorShader->UploadUniformFloat4("u_Color", { 0.65f, 1.0f, 0.51f, 1.0f });

    GenerateTexture2D("assets/textures/tile.png", &m_TileTexture, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT, GL_LINEAR);

    m_TextureUnlitShader = Shader::FromGLSLTextFiles(
        "assets/shaders/textureunlit.vert.glsl",
        "assets/shaders/textureunlit.frag.glsl"
    );

    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformInt("u_Texture", 0);

    glBindVertexArray(0);
}

void StencilSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteBuffers(1, &m_CubeVBO);
    glDeleteBuffers(1, &m_QuadVBO);
}

void StencilSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(StencilSandbox::OnWindowResized));
}

void StencilSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);

    // Floor
    glStencilMask(0x00);
    glBindVertexArray(m_QuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_MetalTexture);
    glm::mat4 modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.1f, 0.0f));
    modelFloor = glm::rotate(modelFloor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelFloor = glm::scale(modelFloor, glm::vec3(7.0f, 7.0f, 1.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", modelFloor);
    m_TextureUnlitShader->UploadUniformFloat("u_TilingFactor", 7.f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw boxes
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF); // enable writing to stencil buffer
    glBindVertexArray(m_CubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TileTexture);
    m_TextureUnlitShader->UploadUniformFloat("u_TilingFactor", 1.f);
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, glm::vec3(-1.0f, 0.0f, -1.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));
    m_TextureUnlitShader->UploadUniformMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw outlines
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST);
    glUseProgram(m_FlatColorShader->GetRendererID());
    m_FlatColorShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
    float scale = 1.1f;
    model1 = glm::scale(model1, glm::vec3(scale, scale, scale));
    m_FlatColorShader->UploadUniformMat4("u_Model", model1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model2 = glm::scale(model2, glm::vec3(scale, scale, scale));
    m_FlatColorShader->UploadUniformMat4("u_Model", model2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
}

void StencilSandbox::OnImGuiRender()
{
}

void StencilSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool StencilSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
