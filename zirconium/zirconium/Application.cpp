#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "imgui/imguiLayer.h"

#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"
#include "log.h"

namespace zirconium {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_OrthoCamera(-1.0f, 1.0f, -1.0f, 1.0f) {
    ZR_ASSERT(!s_Instance, "Application Already Exists!");
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    std::shared_ptr<zirconium::VertexBuffer> VertexBuffer;
    std::shared_ptr<zirconium::IndexBuffer> IndexBuffer;

    std::shared_ptr<zirconium::VertexBuffer> SquareVertexBuffer;
    std::shared_ptr<zirconium::IndexBuffer> SquareIndexBuffer;

    // Create vertex array for the triangle
    m_VertexArray.reset(VertexArray::Create());

    // Triangle vertices with color data
    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.2f, 1.0f, //
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.2f, 1.0f, //
        0.0f,  0.5f,  0.0f, 0.0f, 0.3f, 0.9f, 1.0f, //
    };

    VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    VertexBuffer->Bind();

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
    };
    VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(VertexBuffer);

    // Index Buffer for the triangle
    unsigned int indices[3] = {0, 1, 2};
    IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_VertexArray->SetIndexBuffer(IndexBuffer);

    // // Create vertex array for the square
    // m_SquareVertexArray.reset(VertexArray::Create());

    // // Square vertices (no color, just position)
    // float squareVertices[3 * 4] = {
    //     -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
    // };

    // SquareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    // SquareVertexBuffer->Bind();

    // BufferLayout squareLayout = {
    //     {ShaderDataType::Float3, "a_Position"},
    // };
    // SquareVertexBuffer->SetLayout(squareLayout);
    // m_SquareVertexArray->AddVertexBuffer(SquareVertexBuffer);

    // // Index Buffer for the square
    // unsigned int squareIndices[6] = {0, 1, 2, 2, 1, 3};
    // SquareIndexBuffer.reset(
    //     IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    // m_SquareVertexArray->SetIndexBuffer(SquareIndexBuffer);

    // Shaders
    std::string vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;

    out vec4 v_Color;

    uniform mat4 u_ProjectionViewMatrix;

    void main() {
        gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
        v_Color = a_Color;
    }
  )";

    std::string fragmentShaderSrc = R"(
    #version 330 core
    layout(location = 0) out vec4 color;
    in vec4 v_Color;

    void main() {
        color = v_Color;
    }
  )";

    m_Shader.reset(new Shader(vertexShaderSrc, fragmentShaderSrc));
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
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(onKeyPress));

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

bool Application::onKeyPress(KeyEvent& event) {

    return true;
}

void Application::Run() {
    float rotation = 0.0f;
    while (m_Running) {
        RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        RenderCommand::Clear();
        m_OrthoCamera.SetRotation(rotation++);
        {
            Renderer::BeginScene(m_OrthoCamera);

            Renderer::Submit(m_VertexArray, m_Shader);

            Renderer::EndScene();
        }
        for (Layer* layer : m_layerStack)
            layer->OnUpdate();

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_layerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        // Update window (swap buffers, poll events, etc.)
        m_Window->onUpdate();
    }
}

} // namespace zirconium
