#include "linux.h"
#include "../../core.h"
#include "../../zrpch.h"
#include "../events/Event.h"
#include "../log.h"
#include "../../vendor/glfw/include/GLFW/glfw3.h"

namespace zirconium {
static bool s_GLFWInitialized = false;

Window *Window::Create(const WindowProps &props) {
  return new LinuxWindow(props);
}

void LinuxWindow::init(const WindowProps &props) {
  m_windowData.Title = props.Title;
  m_windowData.Width = props.Width;
  m_windowData.Height = props.Height;

  ZR_CORE_INFO("Creating window: {0} {1} {2}", props.Title, props.Width,
               props.Height);

  if (!s_GLFWInitialized) {
    // TODO: Terminate system shutdown
    int success = glfwInit();
    ZR_CORE_ASSERT(success, "Could not initialize glfw!");
    s_GLFWInitialized = true;
  }
  m_window = glfwCreateWindow((int)props.Width, (int)props.Height,
                              props.Title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_window);
  glfwSetWindowUserPointer(m_window, &m_windowData);
  SetVSync(true);
}
LinuxWindow::LinuxWindow(const WindowProps &props) { init(props); }
LinuxWindow::~LinuxWindow() { shutdown(); }

void LinuxWindow::onUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(m_window);
}
void LinuxWindow::SetEventCallback(const EventCallbackFn &callback) {
  m_windowData.EventCallback = callback;
}

void LinuxWindow::SetVSync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_windowData.VSync = enabled;
}
bool LinuxWindow::IsVSyncOn() const { return m_windowData.VSync; }

void LinuxWindow::shutdown() { glfwDestroyWindow(m_window); }

} // namespace zirconium
