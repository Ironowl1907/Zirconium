#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "GLFW/glfw3.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "log.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace zirconium {

Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));
}
Application::~Application() {}

void Application::onEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

  ZR_CORE_TRACE("{0}", event.ToString());
}

bool Application::onWindowClose(WindowCloseEvent &event) {

  m_Running = false;
  return true;
}

void Application::Run() {
  while (m_Running) {
    glClearColor(0.1804, 0.1804, 0.1804, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_Window->onUpdate();
  }
}

} // namespace zirconium
