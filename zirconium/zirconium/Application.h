#pragma once

#include "../zirconium/window.h"
#include "Renderer/Buffer.h"
#include "events/ApplicationEvent.h"
#include "imgui/imguiLayer.h"
#include "layers/StackLayer.h"
#include <memory>

#include "Renderer/Shader.h"
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
  ImGuiLayer *m_ImGuiLayer;
  bool m_Running = true;
  LayerStack m_layerStack;
  unsigned int m_VertexArray;
  std::unique_ptr<Shader> m_Shader;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;

  static Application *s_Instance;
};
// To be defined in a client
Application *CreateApplication();

} // namespace zirconium
