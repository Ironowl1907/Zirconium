#include "SceneCamera.h"
#include "zrpch.h"

#include "glm/gtc/matrix_transform.hpp"

namespace zirconium {

SceneCamrera::SceneCamrera() {
    // RecalculateProjection();
}

void SceneCamrera::SetOrthographic(float size, float nearClip, float farClip) {
    m_OrthographicSize = size;
    m_OrthographicNear = nearClip;
    m_OrthographicFar = farClip;
    RecalculateProjection();
}

void SceneCamrera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamrera::RecalculateProjection() {
    float orthoLeft = -m_OrthographicSize * 0.5f * m_AspectRatio;
    float orthoRight = m_OrthographicSize * 0.5f * m_AspectRatio;
    float orthoTop = m_OrthographicSize * 0.5f;
    float orthoBottom = -m_OrthographicSize * 0.5f;
    m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
}
} // namespace zirconium
