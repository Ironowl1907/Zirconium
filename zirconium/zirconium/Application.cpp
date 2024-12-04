#include "Application.h"
#include "../core.h"
#include "../zrpch.h"

#include "Renderer/Shader.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "glad/glad.h"
#include "imgui/imguiLayer.h"

#include "Renderer/Shader.h"

namespace zirconium {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application *Application::s_Instance = nullptr;

Application::Application() {
  ZR_ASSERT(!s_Instance, "Application Alredy Exists!");
  s_Instance = this;
  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(BIND_EVENT_FN(onEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);

  // Vertex Array
  glGenVertexArrays(1, &m_VertexArray);
  glBindVertexArray(m_VertexArray);

  // Vertex Buffer
  glGenBuffers(1, &m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

  // clang-format off
  float vertices[3*3] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
  // clang-format on
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  // Index Buffer
  glGenBuffers(1, &m_IndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

  unsigned int indices[3] = {0, 1, 2};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // Shader
  std::string vertexShaderSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main() {
    gl_Position = vec4(a_Position, 1.0);
}
  )";

  std::string fragmentShaderSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(0.8f, 0.2f, 0.3f, 1.0f);
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
