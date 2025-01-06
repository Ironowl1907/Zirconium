#pragma once

#include "zirconium/Core/Timestep.h"
#include "zirconium/Renderer/Camera.h"

#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/KeyEvent.h"
#include "zirconium/Events/MouseEvent.h"

namespace zirconium {

struct OrthoCameraBounds {
    float Left, Right;
    float Up, Down;

    float GetWidth() const {
        return Right - Left;
    }
    float GetHeight() const {
        return Up - Down;
    }
};

class OrthoCameraController {
public:
    OrthoCameraController(const float aspectRatio, const float rotation = false);

    void OnUpdate(TimeStep delta);
    void OnEvent(Event& e);

    void SetZoomLevel(const float& zoom);

    void CalculateView();

    OrthoCamera& GetCamera() {
        return m_Camera;
    }

    const OrthoCamera& GetCamera() const {
        return m_Camera;
    }

    const OrthoCameraBounds& GetBounds() const {
        return m_Bounds;
    }
    void CameraDebugUI();

private:
    bool onMouseScrolled(MouseScrollEvent& e);
    bool onWindowResized(WindowResizeEvent& e);

private:
    float m_AspectRatio;
    bool m_EnableRotation;
    float m_ZoomLevel = 1.0f;

    float m_CameraRotation = 0.0f;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};

    float m_CameraSpeed = m_ZoomLevel, m_CameraRotaionSpeed = 100.0f;

    OrthoCameraBounds m_Bounds;
    OrthoCamera m_Camera;
};
}; // namespace zirconium
