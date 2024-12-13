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
#include "imgui/ImGuiBuild.h"

// Temporary
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imguiKeyMap.h"

namespace zirconium {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    /*io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /*/
    // Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    Application& app = Application::Get();
    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()), true);
    ImGui_ImplOpenGL3_Init("#version 410");
    ZR_INFO("ImGuiLayer attached!");
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        // Restore context
        GLFWwindow* backup_context = glfwGetCurrentContext();
        glfwMakeContextCurrent(backup_context);
    }
}
void ImGuiLayer::OnImGuiRender() {}

} // namespace zirconium
