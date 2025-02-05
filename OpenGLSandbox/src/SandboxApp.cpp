#include "BasicLightingSandbox.h"
#include "StencilSandbox.h"
#include "BlendingSandbox.h"

using namespace OpenGLCore;

class Sandbox : public Application
{
public:
    Sandbox()
    {
        //  PushLayer(new BasicLightingSandbox());
        // PushLayer(new StencilSandbox());
        PushLayer(new BlendingSandbox());
    }
};

int main()
{
    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();
}