#pragma once

#include "Core/Core.h"

#include "Core/Window.h"
#include "Core/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Timestep.h"

#include "ImGui/ImGuiLayer.h"

namespace OpenGLCore {

    class Application
    {
    public:
        Application(const std::string& name = "OpenGL Sandbox", uint32_t width = WINDOW_WIDTH, uint32_t height = WINDOW_HEIGHT);
        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
        float m_LastFrameTime = 0.0f;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    private:
        static Application* s_Instance;
    };

}