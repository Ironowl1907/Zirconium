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

  static inline Application &Get() { return *s_Instance; }
  inline Window &GetWindow() { return *m_Window; }

private:
  bool onWindowClose(WindowCloseEvent &event);
  std::unique_ptr<Window> m_Window;
  bool m_Running = true;
  LayerStack m_layerStack;

  static Application *s_Instance;
};
// To be defined in a client
Application *CreateApplication();

} // namespace zirconium
