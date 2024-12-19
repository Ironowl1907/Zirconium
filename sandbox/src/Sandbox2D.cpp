#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {
    zirconium::Ref<zirconium::VertexBuffer> VertexBuffer;
    zirconium::Ref<zirconium::IndexBuffer> IndexBuffer;

    m_VertexArray = zirconium::VertexArray::Create();

    float vertices[3 * 4] = {
        -0.5f, 0.5f,  0.0f, //
        -0.5f, -0.5f, 0.0f, //
        0.5f,  0.5f,  0.0f, //
        0.5f,  -0.5f, 0.0f, //
    };

    VertexBuffer = zirconium::VertexBuffer::Create(vertices, sizeof(vertices));
    VertexBuffer->Bind();

    zirconium::BufferLayout layout = {
        {zirconium::ShaderDataType::Float3, "a_Position"},
    };
    VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(VertexBuffer);

    unsigned int indices[] = {0, 1, 2, 2, 1, 3};
    IndexBuffer = zirconium::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(IndexBuffer);

    m_FlatColorShader = zirconium::Shader::Create("../sandbox/res/shaders/FlatColorShader.glsl");

    // // Textures
    // m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");

    // std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_Shader)->Bind();
    // std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_Shader)->SetUniformInt("u_Texture", 0);
}
void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {
    // Update
    m_CameraController.OnUpdate(delta);

    // Render
    m_CameraController.OnUpdate(delta);

    zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
    zirconium::RenderCommand::Clear();

    zirconium::Renderer::BeginScene(m_CameraController.GetCamera());

    zirconium::Transform transformation = zirconium::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(1.5f));

    std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<zirconium::OpenGLShader>(m_FlatColorShader)->SetUniformFloat4("u_Color", m_SquareColor);

    zirconium::Renderer::Submit(m_VertexArray, m_FlatColorShader, transformation);

    zirconium::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Scuare Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}
void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_CameraController.OnEvent(event);
}
