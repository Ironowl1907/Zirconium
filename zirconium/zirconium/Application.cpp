#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "GLFW/glfw3.h"
#include "log.h"

namespace zirconium {

Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(std::bind(&Application::onEvent,this, std::placeholders::_1));
}
Application::~Application() {}
void Application::onEvent(Event &event){
  ZR_CORE_INFO("{0}", event);

}

void Application::Run() {
  while (m_Running) {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_Window->onUpdate();
  }
}

} // namespace zirconium
