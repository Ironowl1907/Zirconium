#pragma once

#include "zirconium/Renderer/Camera.h"
#include "zirconium/Core/Timestep.h"

#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/KeyEvent.h"
#include "zirconium/Events/MouseEvent.h"

namespace zirconium {

class OrthoCameraController {
public:
    OrthoCameraController(const float aspectRatio, const float rotation = false);

    void OnUpdate(TimeStep delta);
    void OnEvent(Event& e);

    OrthoCamera& GetCamera() {
        return m_Camera;
    }

    const OrthoCamera& GetCamera() const {
        return m_Camera;
    }

    void CameraDebugUI();

private:
    bool onMouseScrolled(MouseScrollEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    bool m_EnableRotation;

    float m_CameraRotation = 0.0f;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};

    float m_CameraSpeed = m_ZoomLevel, m_CameraRotaionSpeed = 100.0f;

    OrthoCamera m_Camera;
};
}; // namespace zirconium
