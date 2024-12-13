#pragma once
#include "glm/glm.hpp"
#include <glm/geometric.hpp>

#include "imgui.h"

namespace zirconium {
class Camera {
public:
    Camera(float left, float right, float up, float down);

    void SetTarget(glm::vec3 targetPos) {
        m_Target = targetPos;
        m_CameraDirection = glm::normalize(m_SelfPosition - m_Target);
        RecalculateViewMatrix();
    }

    inline void SetPosition(const glm::vec3& position) {
        m_SelfPosition = position;
        RecalculateViewMatrix();
    }

    inline void SetRotation(const float rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    inline const glm::vec3& GetPosition() const {
        return m_SelfPosition;
    }

    inline const float& GetRotation() const {
        return m_Rotation;
    }

    inline const glm::mat4& GetProjectionViewMatrix() const {
        return m_ViewProjtectionMat;
    }

    void CameraDebugUI();

private:
    void RecalculateViewMatrix();

private:
    glm::vec3 m_SelfPosition;
    float m_Rotation = 0.0f;

    glm::mat4 m_ProjectionMat;
    glm::mat4 m_ViewMat;
    glm::mat4 m_ViewProjtectionMat;

    glm::vec3 m_Target;
    glm::vec3 m_CameraDirection;
};

}; // namespace zirconium
