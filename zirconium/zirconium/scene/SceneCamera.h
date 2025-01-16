#pragma once

#include "zirconium/Renderer/Camera.h"

namespace zirconium {

class SceneCamera : public Camera {
public:
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };

    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
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
    }

private:
    void RecalculateProjection();

private:
    ProjectionType m_ProjectionType = ProjectionType::Orthographic;

private:
    float m_OrthographicSize = 10.0f;
    float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

    float m_AspectRatio = 0.0f;
};

} // namespace zirconium
