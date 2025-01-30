#include "SandboxLayer.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

SandboxLayer::SandboxLayer()
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
    EnableGLDebugging();
    SetGLDebugLogLevel(DebugLogLevel::High);
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnEvent(OpenGLCore::Event& event)
{
}

void SandboxLayer::OnUpdate(OpenGLCore::Timestep ts)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");
    ImGui::End();
}
