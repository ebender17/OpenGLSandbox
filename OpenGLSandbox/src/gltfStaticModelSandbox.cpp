#include "gltfStaticModelSandbox.h"

using namespace OpenGLCore;
using namespace OpenGLCore::Utils;

gltfStaticModelSandbox::gltfStaticModelSandbox()
{
    gltfModel::LoadModel(m_Model, "assets/models/Cube/Cube.gltf");
}

gltfStaticModelSandbox::~gltfStaticModelSandbox()
{
}

void gltfStaticModelSandbox::OnAttach()
{
}

void gltfStaticModelSandbox::OnDetach()
{
}

void gltfStaticModelSandbox::OnEvent(Event& event)
{
}

void gltfStaticModelSandbox::OnUpdate(Timestep ts)
{
}

void gltfStaticModelSandbox::OnImGuiRender()
{
}

void gltfStaticModelSandbox::InitializeCamera()
{
}

bool gltfStaticModelSandbox::OnWindowResized(WindowResizeEvent& event)
{
    return false;
}
