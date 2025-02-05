#include "OrthographicCamera.h"
#include "glm/glm.hpp"
#include "zrpch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace zirconium {

OrthoCamera::OrthoCamera(float left, float right, float up, float down)
    : m_SelfPosition({0.0f, 0.0f, 0.0f})
    , m_Rotation(0.0f)
    , m_ProjectionMat(glm::ortho(left, right, up, down, -1.0f, 1.0f))
    , m_ViewMat(1.0f)
    , m_ViewProjectionMat(1.0f)
    , m_Target(0.0f, 0.0f, 0.0f)
    , m_CameraDirection(0.0f, 0.0f, -1.0f) {
    RecalculateViewMatrix();
}

void OrthoCamera::SetProyection(float left, float right, float up, float down) {

    ZR_PROFILE_FUNCTION();

    m_ProjectionMat = glm::ortho(left, right, up, down, -1.0f, 1.0f);
    RecalculateViewMatrix();
}

void OrthoCamera::RecalculateViewMatrix() {

    ZR_PROFILE_FUNCTION();

    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::translate(glm::mat4(1.0f), -m_SelfPosition);

    m_ViewMat = glm::inverse(transform);
    m_ViewProjectionMat = m_ProjectionMat * m_ViewMat;
}

}; // namespace zirconium
