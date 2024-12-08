#pragma once

#include "../zirconium/window.h"
#include "events/ApplicationEvent.h"
#include "imgui/imguiLayer.h"
#include "layers/StackLayer.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "platform/OpenGL/OpenGLBuffer.h"
#include <cmath>

namespace zirconium {

class Application {
public:
    Application();
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

private:
    bool onWindowClose(WindowCloseEvent& event);
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_layerStack;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    std::shared_ptr<Shader> m_Shader2;
    std::shared_ptr<VertexArray> m_SquareVertexArray;

    static Application* s_Instance;
};
// To be defined in a client
Application* CreateApplication();

} // namespace zirconium
