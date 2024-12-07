#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "Renderer/Shader.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "glad/glad.h"
#include "imgui/imguiLayer.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include <GL/gl.h>
#include <cstdint>

namespace zirconium {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application *Application::s_Instance = nullptr;

#include <glad/glad.h> // Include for OpenGL enums like GL_FLOAT

static GLenum ShaderDataTypeToOpenGlBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4:
    return GL_FLOAT;

  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;

  case ShaderDataType::Bool:
    return GL_BOOL;
  }
  ZR_CORE_ASSERT(false, "Unknown RenderAPI!");
  return 0;
}

Application::Application() {
  ZR_ASSERT(!s_Instance, "Application Alredy Exists!");
  s_Instance = this;
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);

  // clang-format off
  float vertices[3*7] = {
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.2f, 1.0f,
    0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.2f, 1.0f,
    0.0f, 0.5f, 0.0f,    0.0f, 0.3f, 0.9f, 1.0f,
  };
  // clang-format on

  m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
  m_VertexBuffer->Bind();
  // clang-format off

  BufferLayout layout = {
    {ShaderDataType::Float3, "a_Position"},
    {ShaderDataType::Float4, "a_Color"},
  };
  // clang-format on

  m_VertexBuffer->SetLayout(layout);

  uint32_t index = 0;
  layout = m_VertexBuffer->GetLayout();

  for (const auto &element : layout) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index++, GetComponentCount(element.Type),
                          ShaderDataTypeToOpenGlBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(), (const void *)element.Offset);
  }

  // Index Buffer
  unsigned int indices[3] = {0, 1, 2};
  m_IndexBuffer.reset(
      IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

  // Shader
  std::string vertexShaderSrc = R"(
#version 330 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec4 a_Color;

    out vec4 v_Color;

    void main() {
        gl_Position = vec4(a_Position, 1.0);
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

void Application::PushLayer(Layer *layer) {
  m_layerStack.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer *overlay) {
  m_layerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

void Application::onEvent(Event &event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

  /*ZR_CORE_TRACE("{0}", event.ToString());*/

  for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
    --it;
    (*it)->OnEvent(event);
    if (event.Handled)
      break;
  }
}

bool Application::onWindowClose(WindowCloseEvent &event) {
  m_Running = false;
  return true;
}

void Application::Run() {
  while (m_Running) {
    glClearColor(0.1804, 0.1804, 0.1804, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    for (Layer *layer : m_layerStack)
      layer->OnUpdate();

    m_ImGuiLayer->Begin();
    for (Layer *layer : m_layerStack)
      layer->OnImGuiRender();
    m_ImGuiLayer->End();

    m_Window->onUpdate();
  }
}

} // namespace zirconium
