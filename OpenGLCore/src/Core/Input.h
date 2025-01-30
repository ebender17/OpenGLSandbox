#pragma once

#include "glpch.h"

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace OpenGLCore {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}