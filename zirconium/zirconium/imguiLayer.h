#pragma once

#include "layers/Layer.h"

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
};

} // namespace zirconium
