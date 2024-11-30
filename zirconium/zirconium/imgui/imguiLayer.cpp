#include "imguiLayer.h"
#include "Application.h"
#include "core.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "layers/Layer.h"

#include "zrpch.h"

#include "imgui.h"
#include "platform/openGl/imGuiOpenGlRenderer.h"


// Temporary
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imguiKeyMap.h"

namespace zirconium {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {}
void ImGuiLayer::OnUpdate() {

  ImGuiIO &io = ImGui::GetIO();
  Application &app = Application::Get();
  io.DisplaySize =
      ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

  float time = (float)glfwGetTime();
  io.DeltaTime = m_time > 0.0 ? (time - m_time) : (1.0f / 60.0f);
  m_time = time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImGuiLayer::Onevent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MousePressedButtonEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnMouseButtonPressedEvent));
  dispatcher.Dispatch<MouseReleasedButtonEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnMouseButtonReleasedEvent));
  dispatcher.Dispatch<MouseMovedEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnMouseMovedEvent));
  dispatcher.Dispatch<MouseScrollEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnMouseScrollEvent));
  dispatcher.Dispatch<WindowResizeEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnWindowResizeEvent));
  dispatcher.Dispatch<KeyPressedEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnKeyPressedEvent));
  dispatcher.Dispatch<KeyReleasedEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnKeyReleasedEvent));
  dispatcher.Dispatch<KeyTypedEvent>(
      ZR_BIND_EVENT_FN(&ImGuiLayer::OnKeyTypedEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MousePressedButtonEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = true;

  return false;
}
bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseReleasedButtonEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = false;

  return false;
}
bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.MousePos = ImVec2(e.getXPosition(), e.getYPosition());

  return false;
}
bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.MouseWheel += e.getYOffset();
  io.MouseWheelH += e.getXOffset();

  return false;
}
bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();


  io.AddKeyEvent(ImGuiKey(MapKeyCodeToImGuiKey(e.GetKeyCode())), true);

  io.AddKeyEvent(ImGuiMod_Ctrl, e.GetKeyCode() == GLFW_KEY_LEFT_CONTROL ||
                                    e.GetKeyCode() == GLFW_KEY_RIGHT_CONTROL);
  io.AddKeyEvent(ImGuiMod_Shift, e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT ||
                                     e.GetKeyCode() == GLFW_KEY_RIGHT_SHIFT);
  io.AddKeyEvent(ImGuiMod_Alt, e.GetKeyCode() == GLFW_KEY_LEFT_ALT ||
                                   e.GetKeyCode() == GLFW_KEY_RIGHT_ALT);
  io.AddKeyEvent(ImGuiMod_Super, e.GetKeyCode() == GLFW_KEY_LEFT_SUPER ||
                                     e.GetKeyCode() == GLFW_KEY_RIGHT_SUPER);

  return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();

  io.AddKeyEvent(ImGuiKey(MapKeyCodeToImGuiKey(e.GetKeyCode())), false);

  return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  int codepoint = e.GetKeyCode();
  io.AddInputCharacter(codepoint);

  return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent &e) {
  ImGuiIO &io = ImGui::GetIO();

  io.DisplaySize = ImVec2(static_cast<GLsizei>(e.GetWidth()),
                          static_cast<GLsizei>(e.GetHeight()));
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  glViewport(0, 0, static_cast<GLsizei>(e.GetWidth()),
             static_cast<GLsizei>(e.GetHeight()));

  return false;
}

} // namespace zirconium
