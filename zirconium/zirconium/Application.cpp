#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "log.h"
#include <GLFW/glfw3.h>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace zirconium {

Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));
}
Application::~Application() {}

void Application::PushLayer(Layer *layer) { m_layerStack.PushLayer(layer); }

void Application::PushOverlay(Layer *overlay) {
  m_layerStack.PushOverlay(overlay);
}

void Application::onEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

  ZR_CORE_TRACE("{0}", event.ToString());

  for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
    --it;
    (*it)->Onevent(event);
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
