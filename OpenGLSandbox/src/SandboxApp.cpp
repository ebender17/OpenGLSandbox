#include "BasicLightingLayer.h"

using namespace OpenGLCore;

class Sandbox : public Application
{
public:
    Sandbox()
    {
        PushLayer(new BasicLightingLayer());
    }
};

int main()
{
    std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
    app->Run();
}