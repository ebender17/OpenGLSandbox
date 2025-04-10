#include "DirectionalShadowMappingSandbox.h"

#include <GLFW/glfw3.h>

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

DirectionalShadowMappingSandbox::DirectionalShadowMappingSandbox()
    : Layer("DirectionalShadowMapping")
{
}

DirectionalShadowMappingSandbox::~DirectionalShadowMappingSandbox()
{
}

void DirectionalShadowMappingSandbox::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::High);

    InitializeCamera();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // Generate depth map
    glGenFramebuffers(1, &m_DepthMapFBO);
    glGenTextures(1, &m_DepthMapTexture);
    glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        c_ShadowMapWidth, c_ShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Depth map framebuffer incomplete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_ShadowMappingDepthShader = Shader::FromGLSLTextFiles(
        "assets/shaders/shadow-mapping/shadow-mapping-depth.vert.glsl",
        "assets/shaders/shadow-mapping/shadow-mapping-depth.frag.glsl"
    );
    ConfigureLightSpaceMatrix();

    // quad VAO
    // TODO : scale instead of setting position here
    float quadVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    GLuint quadVBO;
    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // cube VAO
    float cubeVertices[] = {
        // positions            // normals         // texcoords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    };

    glGenVertexArrays(1, &m_CubeVAO);
    glBindVertexArray(m_CubeVAO);
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glDeleteBuffers(1, &cubeVBO);

    glGenBuffers(1, &m_UBOMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define range of uniform buffer obj to link to uniform binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_ShadowMappingShader = Shader::FromGLSLTextFiles(
        "assets/shaders/shadow-mapping/shadow-mapping.vert.glsl",
        "assets/shaders/shadow-mapping/shadow-mapping.frag.glsl"
    );

    GenerateTexture2D("assets/textures/wooden-box-diffuse.png", &m_WoodenCrateTexture, GL_REPEAT, GL_LINEAR, false);
    glUseProgram(m_ShadowMappingShader->GetRendererID());
    m_ShadowMappingShader->UploadUniformInt("u_DiffuseTexture", 0);
    m_ShadowMappingShader->UploadUniformInt("u_ShadowMap", 1);
}

void DirectionalShadowMappingSandbox::OnDetach()
{
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteVertexArrays(1, &m_CubeVAO);
    glDeleteBuffers(1, &m_UBOMatrices);
    delete m_ShadowMappingDepthShader;
    delete m_ShadowMappingShader;
}

void DirectionalShadowMappingSandbox::OnEvent(OpenGLCore::Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(DirectionalShadowMappingSandbox::OnWindowResized));
}

void DirectionalShadowMappingSandbox::OnUpdate(OpenGLCore::Timestep ts)
{
    // Update
    m_Camera->OnUpdate(ts);

    // uniform buffers
    glm::mat4 viewProjectionMatrix = m_Camera->GetViewProjectionMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProjectionMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // First pass: Render to depth map
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, c_ShadowMapWidth, c_ShadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glUseProgram(m_ShadowMappingDepthShader->GetRendererID());
    m_ShadowMappingDepthShader->UploadUniformMat4("u_LightSpaceMatrix", m_LightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodenCrateTexture);
    RenderScene(*m_ShadowMappingDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Second pass: Render scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 camPos = m_Camera->GetPosition();
    m_ShadowMappingShader->UploadUniformFloat3("u_ViewPos", camPos);
    m_ShadowMappingShader->UploadUniformFloat3("u_LightPos", m_LightPos);
    m_ShadowMappingShader->UploadUniformMat4("u_LightSpaceMatrix", m_LightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_WoodenCrateTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
    RenderScene(*m_ShadowMappingShader);
}

void DirectionalShadowMappingSandbox::OnImGuiRender()
{
}

void DirectionalShadowMappingSandbox::InitializeCamera()
{
    PerspectiveProjInfo persProjInfo = { 45.0f, (float)1280, (float)720, 0.1f, 1000.0f };
    m_Camera = std::make_unique<FirstPersonCamera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), persProjInfo);
}

bool DirectionalShadowMappingSandbox::OnWindowResized(OpenGLCore::WindowResizeEvent& event)
{
    unsigned int width = event.GetWidth();
    unsigned int height = event.GetHeight();

    m_Camera->SetAspectRatio(width, height);
    glViewport(0, 0, width, height);
    return false;
}

void DirectionalShadowMappingSandbox::ConfigureLightSpaceMatrix()
{
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(m_LightPos,
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    m_LightSpaceMatrix = lightProjection * lightView;
}

void DirectionalShadowMappingSandbox::RenderScene(OpenGLCore::Utils::Shader& shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.UploadUniformMat4("u_Model", model);
    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    glBindVertexArray(m_CubeVAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.UploadUniformMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.UploadUniformMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.UploadUniformMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
