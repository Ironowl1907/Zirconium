#include "Application.h"
#include "core.h"
#include "window.h"
#include "zrpch.h"

#include "zirconium/Core/Application.h"
#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/Event.h"
#include "zirconium/ImGui/imguiLayer.h"

#include "zirconium/Debug/Instrumentor.h"

#include "zirconium/Renderer/Renderer.h"

#include "zirconium/Core/Timestep.h"
#include <GLFW/glfw3.h>

namespace zirconium {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;


Application::Application(const std::string& name, ApplicationCommandLineArgs args)
    : m_CommandLineArgs(args) {

    ZR_PROFILE_FUNCTION();

    ZR_ASSERT(!s_Instance, "Application Already Exists!");
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
    m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::PushLayer(Layer* layer) {

    ZR_PROFILE_FUNCTION();

    m_layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {

    ZR_PROFILE_FUNCTION();

    m_layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::onEvent(Event& event) {

    ZR_PROFILE_FUNCTION();

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

    for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
        --it;
        (*it)->OnEvent(event);
        if (event.Handled)
            break;
    }
}

bool Application::onWindowResize(WindowResizeEvent& event) {
    if (event.GetWidth() == 0 || event.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }
    m_Minimized = false;
    Renderer::OnWindowResize(0, 0, event.GetWidth(), event.GetHeight());

    return false;
}

bool Application::onWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

void Application::Run() {

    ZR_PROFILE_FUNCTION();

    while (m_Running) {
        ZR_PROFILE_SCOPE("RunLoop");
        float time = glfwGetTime();
        TimeStep deltaTime(time - m_LastFrameTime);
        m_LastFrameTime = time;

        if (!m_Minimized) {
            {
                ZR_PROFILE_SCOPE("Layers OnUpdate Application::Run(void)");
                for (Layer* layer : m_layerStack)
                    layer->OnUpdate(deltaTime);
            }
            m_ImGuiLayer->Begin();
            {
                ZR_PROFILE_SCOPE("Layers OnImGuiRender Application::Run(void");
                for (Layer* layer : m_layerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
        }

        // Update window (swap buffers, poll events, etc.)
        m_Window->onUpdate();
    }
}
void Application::Close() {
    m_Running = false;
}

} // namespace zirconium
