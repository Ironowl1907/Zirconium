#pragma once

#include "../events/ApplicationEvent.h"
#include "../events/KeyEvent.h"
#include "../events/MouseEvent.h"
#include "../layers/Layer.h"

namespace zirconium {
class ImGuiLayer : public Layer {
public:
  ImGuiLayer();
  ~ImGuiLayer();

  void OnAttach();
  void OnDetach();
  void OnUpdate();
  void Onevent(Event &event);

private:
  float m_time;

  bool OnMouseButtonPressedEvent(MousePressedButtonEvent &e);
  bool OnMouseButtonReleasedEvent(MouseReleasedButtonEvent &e);
  bool OnMouseMovedEvent(MouseMovedEvent &e);
  bool OnMouseScrollEvent(MouseScrollEvent &e);
  bool OnWindowResizeEvent(WindowResizeEvent &e);
  bool OnKeyPressedEvent(KeyPressedEvent &e);
  bool OnKeyReleasedEvent(KeyReleasedEvent &e);
  bool OnKeyTypedEvent(KeyTypedEvent &e);
};

} // namespace zirconium
