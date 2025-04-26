#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core/Timestep.h"

namespace OpenGLCore::Utils {

    static bool constexpr CAMERA_LEFT_HANDED = true;

    struct PerspectiveProjInfo
    {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    };

    class FirstPersonCamera
    {
    public:
        FirstPersonCamera(const glm::vec3& position, const glm::vec3& target,
            const glm::vec3& up, PerspectiveProjInfo& perspectiveProjInfo);

        void OnUpdate(Timestep timestep);

        void CalculateVelocity(float deltaTime);
        glm::vec3 CalculateAcceleration();
        void CalculateCameraOrientation();

        void SetAspectRatio(unsigned int width, unsigned int height);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetViewProjectionMatrix() const;
        glm::vec3 GetForward() const;
        void SetUpVector();

        const glm::mat4& GetProjMatrix() const { return m_PerspectiveProj; }
        const glm::vec3& GetPosition() const { return m_CameraPos; }
    private:
        void UpdateProjectionMatrix();
        glm::vec2 GetNormalizedMousePosition();
    private:
        PerspectiveProjInfo m_PerspectiveProjInfo;
        glm::vec3 m_CameraPos;
        glm::vec3 m_Target;
        glm::vec3 m_Up;
        glm::mat4 m_PerspectiveProj = glm::mat4(1.0f);
        glm::quat m_CameraOrientation = glm::quat(glm::vec3(0.0f));

        float m_AccelerationFactor = 150.0f;
        float m_Damping = 5.0f;
        float m_MaxSpeed = 2.0f;
        float m_FastCoef = 10.0f;
        glm::vec3 m_Velocity = glm::vec3(0.0f);

        glm::vec2 m_MousePos = glm::vec2(1.0f);
        float m_MouseSpeed = 4.0f;
    };

}