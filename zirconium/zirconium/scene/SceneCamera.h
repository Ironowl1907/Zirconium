#pragma once

#include "zirconium/Renderer/Camera.h"

namespace zirconium {

class SceneCamera : public Camera {
public:
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };

    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetPerspective(float verticalFOV, float nearClip, float farClip);

    // Perspective
    float GetPerspectiveVerticalFOV() const {
        return m_PerspectiveFOV;
    }
    void SetPerspectiveFOV(const float& fov) {
        m_PerspectiveFOV = fov;
        RecalculateProjection();
    }
    float GetPerspectiveNearClip() const {
        return m_PerspectiveNear;
    }
    void SetPerspectiveNearClip(const float near) {
        m_PerspectiveNear = near;
        RecalculateProjection();
    }
    float GetPerspectiveFarClip() const {
        return m_PerspectiveFar;
    }
    void SetPerspectiveFarClip(const float far) {
        m_PerspectiveFar = far;
        RecalculateProjection();
    }

    // Orthographic
    void SetViewportSize(uint32_t width, uint32_t height);
    float GetOrthographicSize() const {
        return m_OrthographicSize;
    }
    float GetOrthographicNearClip() const {
        return m_OrthographicNear;
    }
    void SetOrthographicNearClip(const float near) {
        m_OrthographicNear = near;
        RecalculateProjection();
    }
    float GetOrthographicFarClip() const {
        return m_OrthographicFar;
    }
    void SetOrthographicFarClip(const float far) {
        m_OrthographicFar = far;
        RecalculateProjection();
    }
    void SetOrthographicSize(const float& size) {
        m_OrthographicSize = size;
        RecalculateProjection();
    }

    ProjectionType GetProjectionType() const {
        return m_ProjectionType;
    }
    void SetProjectionType(ProjectionType type) {
        m_ProjectionType = type;
        RecalculateProjection();
    }

private:
    void RecalculateProjection();

private:
    ProjectionType m_ProjectionType = ProjectionType::Orthographic;

private:
    float m_PerspectiveFOV = glm::radians(45.0f);
    float m_PerspectiveNear = -0.01f, m_PerspectiveFar = 1000.0f;

    float m_OrthographicSize = 10.0f;
    float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

    float m_AspectRatio = 0.0f;
};

} // namespace zirconium
