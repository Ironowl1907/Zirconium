#include "Application.h"
#include "core.h"
#include "zrpch.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "imgui/imguiLayer.h"

#include "Renderer/Renderer.h"

#include "GLFW/glfw3.h"
#include "Timestep.h"

namespace zirconium {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
    ZR_ASSERT(!s_Instance, "Application Already Exists!");
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::PushLayer(Layer* layer) {
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

    for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
        --it;
        (*it)->OnEvent(event);
        if (event.Handled)
            break;
    }
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

void Application::Run() {
    while (m_Running) {
        float time = glfwGetTime();
        TimeStep deltaTime(time - m_LastFrameTime);
        m_LastFrameTime = time;
        for (Layer* layer : m_layerStack)
            layer->OnUpdate(deltaTime);

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_layerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        // Update window (swap buffers, poll events, etc.)
        m_Window->onUpdate();
    }
}

} // namespace zirconium
