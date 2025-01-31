#include "glpch.h"
#include "FirstPersonCamera.h"

#include <algorithm>

#include "Core/Input.h"

namespace OpenGLCore::Utils {

    FirstPersonCamera::FirstPersonCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, PerspectiveProjInfo& perspectiveProjInfo)
        : m_PerspectiveProjInfo(perspectiveProjInfo), m_CameraPos(position), m_Target(target), m_Up(up)
    {
        float aspectRatio = m_PerspectiveProjInfo.Width / m_PerspectiveProjInfo.Height;

        if (CAMERA_LEFT_HANDED)
        {
            m_CameraOrientation = glm::lookAtLH(m_CameraPos, m_Target, m_Up);
            m_PerspectiveProj = glm::perspectiveLH(m_PerspectiveProjInfo.FOV, aspectRatio,
                m_PerspectiveProjInfo.zNear, m_PerspectiveProjInfo.zFar);
            return;
        }
        m_CameraOrientation = glm::lookAtRH(m_CameraPos, m_Target, m_Up);
        m_PerspectiveProj = glm::perspectiveRH(m_PerspectiveProjInfo.FOV, aspectRatio,
            m_PerspectiveProjInfo.zNear, m_PerspectiveProjInfo.zFar);
    }

    void FirstPersonCamera::OnUpdate(Timestep timestep)
    {
        float deltaTime = timestep.GetSeconds();
        CalculateCameraOrientation();
        m_MousePos = GetNormalizedMousePosition();
        CalculateVelocity(deltaTime);
        m_CameraPos += m_Velocity * deltaTime;
    }

    void FirstPersonCamera::CalculateVelocity(float deltaTime)
    {
        glm::vec3 acceleration = CalculateAcceleration();
        bool isUsingFastSpeed = false;
        if (Input::IsKeyPressed(Key::LeftShift))
        {
            isUsingFastSpeed = true;
            acceleration *= m_FastCoef;
        }

        if (acceleration == glm::vec3(0.0))
            m_Velocity -= m_Velocity * std::min(deltaTime * m_Damping, 1.0f);
        else
        {
            m_Velocity += acceleration * m_AccelerationFactor * deltaTime;
            float maxSpeed = isUsingFastSpeed ? m_MaxSpeed * m_FastCoef : m_MaxSpeed;

            if (glm::length(m_Velocity) > m_MaxSpeed)
                m_Velocity = glm::normalize(m_Velocity) * m_MaxSpeed;
        }
    }

    glm::vec3 FirstPersonCamera::CalculateAcceleration()
    {
        glm::mat4 orientation = glm::mat4_cast(m_CameraOrientation);
        glm::vec3 forward, up;
        glm::vec3 right = glm::vec3(orientation[0][0], orientation[1][0], orientation[2][0]); // glm uses column-major matrices

        if (CAMERA_LEFT_HANDED)
        {
            forward = glm::vec3(orientation[0][2], orientation[1][2], orientation[2][2]);
            up = glm::cross(right, forward);
        }
        else
        {
            forward = -glm::vec3(orientation[0][2], orientation[1][2], orientation[2][2]);
            up = glm::cross(forward, right);
        }

        glm::vec3 acceleration = glm::vec3(0.0f);

        if (Input::IsKeyPressed(Key::A))
        {
            acceleration += -right;
        }
        if (Input::IsKeyPressed(Key::D))
        {
            acceleration += right;
        }
        if (Input::IsKeyPressed(Key::W))
        {
            acceleration += forward;
        }
        if (Input::IsKeyPressed(Key::S))
        {
            acceleration += -forward;
        }
        if (Input::IsKeyPressed(Key::Q))
        {
            acceleration += up;
        }
        if (Input::IsKeyPressed(Key::E))
        {
            acceleration += -up;
        }

        return acceleration;
    }

    void FirstPersonCamera::CalculateCameraOrientation()
    {
        if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
        {
            glm::vec2 mousePos = GetNormalizedMousePosition();

            glm::vec2 deltaMouseMove = glm::vec2(mousePos.x, mousePos.y) - m_MousePos;

            glm::quat deltaQuat = glm::quat(glm::vec3(m_MouseSpeed * deltaMouseMove.y, m_MouseSpeed * deltaMouseMove.x, 0.0f));

            m_CameraOrientation = glm::normalize(deltaQuat * m_CameraOrientation);

            SetUpVector();
        }
    }

    void FirstPersonCamera::SetAspectRatio(unsigned int width, unsigned int height)
    {
        m_PerspectiveProjInfo.Width = (float)width;
        m_PerspectiveProjInfo.Height = (float)height;
        UpdateProjectionMatrix();
    }

    // aka the camera
    glm::mat4 FirstPersonCamera::GetViewMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0), -m_CameraPos);
        glm::mat4 rotation = glm::mat4_cast(m_CameraOrientation);
        return rotation * translation;
    }

    glm::mat4 FirstPersonCamera::GetViewProjectionMatrix() const
    {
        return m_PerspectiveProj * GetViewMatrix();
    }

    glm::vec3 FirstPersonCamera::GetForward() const
    {
        glm::mat4 view = GetViewMatrix();
        return glm::vec3(view[0][2], view[1][2], view[2][2]);
    }

    void FirstPersonCamera::SetUpVector()
    {
        // Disables rotation around z-axis, no roll
        glm::vec3 forward = GetForward();

        if ((CAMERA_LEFT_HANDED))
        {
            m_CameraOrientation = glm::lookAtLH(m_CameraPos, m_CameraPos + forward, m_Up);
            return;
        }
        m_CameraOrientation = glm::lookAtRH(m_CameraPos, m_CameraPos - forward, m_Up);
    }

    void FirstPersonCamera::UpdateProjectionMatrix()
    {
        float aspectRatio = m_PerspectiveProjInfo.Width / m_PerspectiveProjInfo.Height;

        if (CAMERA_LEFT_HANDED)
        {
            m_PerspectiveProj = glm::perspectiveLH(m_PerspectiveProjInfo.FOV, aspectRatio,
                m_PerspectiveProjInfo.zNear, m_PerspectiveProjInfo.zFar);
            return;
        }
        m_PerspectiveProj = glm::perspectiveRH(m_PerspectiveProjInfo.FOV, aspectRatio,
            m_PerspectiveProjInfo.zNear, m_PerspectiveProjInfo.zFar);
    }

    glm::vec2 FirstPersonCamera::GetNormalizedMousePosition()
    {
        auto [x, y] = Input::GetMousePosition();
        float mouseXPosNormalized = x / m_PerspectiveProjInfo.Width;
        float mouseYPosNormalized = y / m_PerspectiveProjInfo.Height;
        return { mouseXPosNormalized, mouseYPosNormalized };
    }

}