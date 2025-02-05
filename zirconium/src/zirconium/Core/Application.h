#pragma once

#include "zirconium/Core/Timestep.h"
#include "zirconium/Core/window.h"
#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/KeyEvent.h"
#include "zirconium/ImGui/imguiLayer.h"
#include "zirconium/Layers/StackLayer.h"

#include "zirconium/Renderer/Buffer.h"
#include "zirconium/Renderer/Shader.h"
#include "zirconium/Renderer/VertexArray.h"

#include "zirconium/Platform/OpenGL/OpenGLBuffer.h"

namespace zirconium {

class Application {
public:
    Application(const std::string& name = "Zirconium App");
    virtual ~Application();
    void Run();
    void onEvent(Event& event);
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    static inline Application& Get() {
        return *s_Instance;
    }
    inline Window& GetWindow() {
        return *m_Window;
    }

    void Close();

    ImGuiLayer* GetImGuiLayer() {
        return m_ImGuiLayer;
    }

private:
    bool onWindowClose(WindowCloseEvent& event);
    bool onWindowResize(WindowResizeEvent& event);
    bool onKeyPress(KeyEvent& event);
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    bool m_Minimized = false;
    LayerStack m_layerStack;

    float m_LastFrameTime;

    uint32_t m_UniformProjViewLocation;

    static Application* s_Instance;
};
// To be defined in a client
Application* CreateApplication();

} // namespace zirconium
