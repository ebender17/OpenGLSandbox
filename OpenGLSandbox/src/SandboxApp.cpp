#include "BasicLightingSandbox.h"
#include "StencilSandbox.h"
#include "BlendingSandbox.h"
#include "PostProcessSandbox.h"
#include "PostProcessMSAASandbox.h"
#include "gltfStaticModelSandbox.h"

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
        PushLayer(new gltfStaticModelSandbox());
    }
};

int main()
{
    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();
}