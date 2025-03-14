#include "BasicLightingSandbox.h"
#include "StencilSandbox.h"
#include "BlendingSandbox.h"
#include "PostProcessSandbox.h"
#include "PostProcessMSAASandbox.h"
#include "gltfModelSandbox.h"
#include "CubemapSandbox.h"
#include "BlinnPhongSandbox.h"
#include "InstancingSandbox.h"
#include "GeometryShaderSandbox.h"

using namespace OpenGLCore;

class Sandbox : public Application
{
public:
    Sandbox()
    {
        // PushLayer(new BasicLightingSandbox());
        // PushLayer(new StencilSandbox());
        // PushLayer(new BlendingSandbox());
        // PushLayer(new PostProcessSandbox());
        // PushLayer(new PostProcessMSAASandbox());
        // PushLayer(new gltfModelSandbox());
        // PushLayer(new CubemapSandbox());
        // PushLayer(new BlinnPhongSandbox());
        // PushLayer(new InstancingSandbox());
        PushLayer(new GeometryShaderSandbox());
    }
};

int main()
{
    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();
}