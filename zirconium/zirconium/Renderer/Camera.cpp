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
    // Rotate the camera first, then translate it
    glm::mat4 transform =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::translate(glm::mat4(1.0f), -m_SelfPosition);

    m_ViewMat = glm::inverse(transform);                // Inverse for view matrix
    m_ViewProjtectionMat = m_ProjectionMat * m_ViewMat; // Final view-projection matrix
}

void Camera::CameraDebugUI() {
    ImGui::Begin("Camera Debugger");

    // Position Controls
    glm::vec3 position = GetPosition();
    if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
        SetPosition(position);
    }

    // Rotation Controls
    float rotation = GetRotation();
    if (ImGui::DragFloat("Rotation", &rotation, 0.1f, -360.0f, 360.0f)) {
        SetRotation(rotation);
    }

    // ViewProjection Matrix Display
    ImGui::Text("Projection-View Matrix:");
    const glm::mat4& viewProjMat = GetProjectionViewMatrix();
    for (int i = 0; i < 4; ++i) {
        ImGui::Text("%.2f %.2f %.2f %.2f", viewProjMat[i][0], viewProjMat[i][1], viewProjMat[i][2],
                    viewProjMat[i][3]);
    }
    ImGui::Text("Target");
    ImGui::Text("%.2f %.2f %.2f", m_Target.x, m_Target.y, m_Target.z);

    ImGui::End();
}
}; // namespace zirconium
