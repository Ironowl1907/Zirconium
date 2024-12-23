#include "zrpch.h"

#include "core.h"

#include "imgui.h"
#include "zirconium/Renderer/OrthoCameraController.h"

#include "zirconium/Core/input.h"
#include "zirconium/Core/keycodes.h"

namespace zirconium {

OrthoCameraController::OrthoCameraController(const float aspectRatio, const float rotation)
    : m_AspectRatio(aspectRatio)
    , m_EnableRotation(rotation)
    , m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel) {}

void OrthoCameraController::OnUpdate(TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    if (Input::IsKeyPressed(ZR_KEY_W)) {
        m_CameraPosition.y -= m_CameraSpeed * delta;
    } else if (Input::IsKeyPressed(ZR_KEY_S)) {
        m_CameraPosition.y += m_CameraSpeed * delta;
    }
    if (Input::IsKeyPressed(ZR_KEY_D)) {
        m_CameraPosition.x -= m_CameraSpeed * delta;
    } else if (Input::IsKeyPressed(ZR_KEY_A)) {
        m_CameraPosition.x += m_CameraSpeed * delta;
    }

    if (m_EnableRotation) {
        if (Input::IsKeyPressed(ZR_KEY_Q)) {
            m_CameraRotation += m_CameraRotaionSpeed * delta;
        } else if (Input::IsKeyPressed(ZR_KEY_E)) {
            m_CameraRotation -= m_CameraRotaionSpeed * delta;
        }
        m_Camera.SetRotation(m_CameraRotation);
    }
    m_Camera.SetPosition(m_CameraPosition);
}
void OrthoCameraController::OnEvent(Event& e) {

    ZR_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<MouseScrollEvent>(ZR_BIND_EVENT_FN(OrthoCameraController::onMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(ZR_BIND_EVENT_FN(OrthoCameraController::onWindowResized));
}

bool OrthoCameraController::onMouseScrolled(MouseScrollEvent& e) {
    m_ZoomLevel -= e.getYOffset() * 0.1f;
    m_ZoomLevel = std::max(0.25f, m_ZoomLevel);
    m_Camera.SetProyection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

    m_CameraSpeed = m_ZoomLevel;
    return false;
}
bool OrthoCameraController::onWindowResized(WindowResizeEvent& e) {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProyection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}

void OrthoCameraController::CameraDebugUI() {

    ZR_PROFILE_FUNCTION();

    ImGui::Begin("Camera Debugger");

    glm::vec3 position = m_Camera.GetPosition();
    if (ImGui::DragFloat3("Position", &position.x, 0.1f)) {
        m_Camera.SetPosition(position);
    }

    float rotation = m_Camera.GetRotation();
    if (ImGui::DragFloat("Rotation", &rotation, 0.1f, -360.0f, 360.0f)) {
        m_Camera.SetRotation(rotation);
    }

    ImGui::Text("Projection-View Matrix:");
    const glm::mat4& viewProjMat = m_Camera.GetProjectionViewMatrix();
    for (int i = 0; i < 4; ++i) {
        ImGui::Text("%.2f %.2f %.2f %.2f", viewProjMat[i][0], viewProjMat[i][1], viewProjMat[i][2], viewProjMat[i][3]);
    }

    const glm::vec3& target = m_Camera.GetTarget();
    ImGui::Text("Target");
    ImGui::Text("%.2f %.2f %.2f", target.x, target.y, target.z);

    ImGui::Text("Zoom");
    ImGui::Text("%.2f", m_ZoomLevel);

    ImGui::End();
}

} // namespace zirconium
