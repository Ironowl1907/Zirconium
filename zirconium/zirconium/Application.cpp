#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "glad/glad.h"
#include "log.h"

#include "input.h"

namespace zirconium {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application *Application::s_Instance = nullptr;

Application::Application() {
  ZR_ASSERT(!s_Instance, "Application Alredy Exists!");
  s_Instance = this;
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

  unsigned int id;
  glGenVertexArrays(1, &id);
}
Application::~Application() {}

void Application::PushLayer(Layer *layer) {
  m_layerStack.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer *overlay) {
  m_layerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

void Application::onEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

  ZR_CORE_TRACE("{0}", event.ToString());

  for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
    --it;
    (*it)->OnEvent(event);
    if (event.Handled)
      break;
  }
}

bool Application::onWindowClose(WindowCloseEvent &event) {

  m_Running = false;
  return true;
}

void Application::Run() {
  while (m_Running) {
    glClearColor(0.1804, 0.1804, 0.1804, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Layer *layer : m_layerStack)
      layer->OnUpdate();

    m_Window->onUpdate();
  }
}

} // namespace zirconium
