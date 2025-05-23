#pragma once
#include "Camera.h"
#include "zirconium/Core/Timestep.h"
#include "zirconium/Events/Event.h"
#include "zirconium/Events/MouseEvent.h"

// clang-format off
namespace zirconium {
class EditorCamera : public Camera {

public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(TimeStep delta);
    void OnEvent(Event& e);

    inline float GetDistance() const { return m_Distance; }
    inline void SetDistance(const float distance) { m_Distance = distance; }

    inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
    glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    const glm::vec3& GetPosition() const { return m_Position; }
    glm::quat GetOrientation() const;

    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }


private:
    void UpdateProjection();
    void UpdateView();

    bool OnMouseScroll(MouseScrollEvent& e);

    void OnMousePan(const glm::vec2& delta);
    void OnMouseRotate(const glm::vec2& delta);
    void OnZoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float,float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

private:
    float m_FOV = 45.0f, m_AspectRatio = 1.788f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

    glm::mat4 m_ViewMatrix;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};

    glm::vec2 m_InitialMousePosition;

    float m_Distance = 10.0f;
    float m_Pitch = 0.0f, m_Yaw = 0.0f;

    float m_ViewportWidth = 1280, m_ViewportHeight = 720;
};

} // namespace zirconium
