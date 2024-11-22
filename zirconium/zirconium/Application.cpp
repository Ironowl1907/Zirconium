#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "GLFW/glfw3.h"
#include <GL/gl.h>

namespace zirconium {

Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
}
Application::~Application() {}

void Application::Run() {
  while (m_Running){
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
     m_Window->onUpdate();
  }
}

} // namespace zirconium
