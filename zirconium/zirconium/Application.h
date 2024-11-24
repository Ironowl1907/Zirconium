#pragma once

#include "../zirconium/window.h"
#include "events/ApplicationEvent.h"
#include "layers/StackLayer.h"
#include <memory>
namespace zirconium {

class Application {

public:
  Application();
  virtual ~Application();
  void Run();
  void onEvent(Event &event);
  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);

private:
  bool onWindowClose(WindowCloseEvent &event);
  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
  LayerStack m_layerStack;
};
// To be defined in a client
Application *CreateApplication();

} // namespace zirconium
