#include "SceneCamera.h"
#include "zrpch.h"

#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> // For glm::to_string

namespace zirconium {

SceneCamera::SceneCamera() {
    // RecalculateProjection();
}

void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip) {
    m_ProjectionType = ProjectionType::Perspective;
    m_PerspectiveFOV = verticalFOV;
    m_PerspectiveFar = farClip;
    m_PerspectiveNear = nearClip;
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    m_ProjectionType = ProjectionType::Orthographic;
    m_OrthographicSize = size;
    m_OrthographicNear = nearClip;
    m_OrthographicFar = farClip;
    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    if (width == 0 || height == 0)
        m_AspectRatio = 1.0f;
    else
        m_AspectRatio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    // std::cout << "m_PerspectiveFOV: " << m_PerspectiveFOV << std::endl;
    // std::cout << "m_PerspectiveNear: " << m_PerspectiveNear << std::endl;
    // std::cout << "m_PerspectiveFar: " << m_PerspectiveFar << std::endl;

    // std::cout << "m_OrthographicSize: " << m_OrthographicSize << std::endl;
    // std::cout << "m_OrthographicNear: " << m_OrthographicNear << std::endl;
    // std::cout << "m_OrthographicFar: " << m_OrthographicFar << std::endl;

    // std::cout << "m_AspectRatio: " << m_AspectRatio << std::endl;

    if (m_ProjectionType == ProjectionType::Orthographic) {
        float orthoLeft = -m_OrthographicSize * 0.5f * m_AspectRatio;
        float orthoRight = m_OrthographicSize * 0.5f * m_AspectRatio;
        float orthoTop = m_OrthographicSize * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    } else {
        m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }
}
} // namespace zirconium
