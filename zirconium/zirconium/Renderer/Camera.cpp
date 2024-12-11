#include "Camera.h"
#include "glm/glm.hpp"
#include "zrpch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zirconium {

Camera::Camera(float left, float right, float up, float down)
    : m_ProjectionMat(glm::ortho(left, right, up, down, -1.0f, 1.0f))
    , m_SelfPosition({0.0f, 0.0f, 0.0f})
    , m_ViewMat(1.0f) {
    RecalculateViewMatrix();
}

void Camera::RecalculateViewMatrix() {
    // First rotate the camera around the origin, then translate it to its position
    glm::mat4 transform =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *  // Rotation first
        glm::translate(glm::mat4(1.0f), -m_SelfPosition);  // Translate the camera position after rotation

    m_ViewMat = glm::inverse(transform);  // Inverse for view matrix
    m_ViewProjtectionMat = m_ViewMat * m_ProjectionMat;  // Final view-projection matrix
}

}; // namespace zirconium
