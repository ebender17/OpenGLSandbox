#include "BlendingSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

BlendingSandbox::BlendingSandbox()
    : Layer("BlendingSandbox")
{
}

BlendingSandbox::~BlendingSandbox()
{
}

void BlendingSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::Notification);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

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

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_FlatColorShader = Shader::FromGLSLTextFiles(
        "assets/shaders/flatcolor.vert.glsl",
        "assets/shaders/flatcolor.frag.glsl"
    );
    glUseProgram(m_FlatColorShader->GetRendererID());
    m_FlatColorShader->UploadUniformFloat4("u_Color", { 0.65f, 1.0f, 0.51f, 1.0f });

    GenerateTexture2D("assets/textures/tile.png", &m_TileTexture, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/metal.png", &m_MetalTexture, GL_REPEAT, GL_LINEAR);
    GenerateTexture2D("assets/textures/transparent-window.png", &m_TransparentWindow, GL_CLAMP, GL_LINEAR);
    GenerateTexture2D("assets/textures/flat-grass-sprite.png", &m_GrassSpriteTexture, GL_CLAMP, GL_LINEAR);

    m_TextureUnlitShader = Shader::FromGLSLTextFiles(
        "assets/shaders/textureunlit.vert.glsl",
        "assets/shaders/textureunlit.frag.glsl"
    );

    glUseProgram(m_TextureUnlitShader->GetRendererID());
    m_TextureUnlitShader->UploadUniformInt("u_Texture", 0);

    m_AlphaClipShader = Shader::FromGLSLTextFiles(
        "assets/shaders/alphacliptexture.vert.glsl",
        "assets/shaders/alphacliptexture.frag.glsl"
    );

    glUseProgram(m_AlphaClipShader->GetRendererID());
    m_AlphaClipShader->UploadUniformFloat("u_AlphaThreshold", 0.1f);
    m_AlphaClipShader->UploadUniformFloat4("u_Color", { 0.09f, 0.73f, 0.47f, 1.0f });

    m_VegetationPositions.reserve(5);
    m_VegetationPositions.emplace_back(glm::vec3(-1.5f, -0.1f, 0.5f));
    m_VegetationPositions.emplace_back(glm::vec3(1.5f, -0.1f, 1.2f));
    m_VegetationPositions.emplace_back(glm::vec3(0.0f, -0.1f, 0.7f));
    m_VegetationPositions.emplace_back(glm::vec3(-0.3f, -0.1f, -2.3f));
    m_VegetationPositions.emplace_back(glm::vec3(0.5f, -0.1f, -0.6f));

    m_WindowPositions.reserve(2);
    m_WindowPositions.emplace_back(glm::vec3(-1.0f, 0.0f, 0.01f));
    m_WindowPositions.emplace_back(glm::vec3(1.0f, 0.0f, -1.0f));

    glBindVertexArray(0);
}

void BlendingSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteVertexArrays(1, &m_QuadVAO);

    delete m_FlatColorShader;
    delete m_TextureUnlitShader;
}

void BlendingSandbox::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(BlendingSandbox::OnWindowResized));
}

void BlendingSandbox::OnUpdate(Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // Render
    glClearColor(0.1f, 0.5f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Floor
    glUseProgram(m_TextureUnlitShader->GetRendererID());
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

    // Vegetation
    glBindVertexArray(m_QuadVAO);
    glUseProgram(m_AlphaClipShader->GetRendererID());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GrassSpriteTexture);
    for (glm::vec3 position : m_VegetationPositions)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        m_AlphaClipShader->UploadUniformMat4("u_Model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // Windows (from farthest to nearest to solve depth issues)
    std::map<float, glm::vec3> sortedWindows;
    for (glm::vec3 position : m_WindowPositions)
    {
        glm::vec3 difference = m_Camera->GetPosition() - position;
        float sqrMagnitude = glm::dot(difference, difference);
        sortedWindows[sqrMagnitude] = position;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TransparentWindow);
    for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); ++it)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        m_AlphaClipShader->UploadUniformMat4("u_Model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

void BlendingSandbox::OnImGuiRender()
{
}

void BlendingSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool BlendingSandbox::OnWindowResized(WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}
