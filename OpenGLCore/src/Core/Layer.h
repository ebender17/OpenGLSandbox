#pragma once

#include "Core.h"
#include "Timestep.h"
#include "Events/Event.h"

namespace OpenGLCore {

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnImGuiRender() {}

        const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}