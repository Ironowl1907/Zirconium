#include "imguiLayer.h"
#include "Application.h"
#include "layers/Layer.h"
#include "zrpch.h"

#include "imgui.h"
#include "platform/openGl/imGuiOpenGlRenderer.h"
#include <GLFW/glfw3.h>

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
void ImGuiLayer::Onevent(Event &event) {}

} // namespace zirconium
