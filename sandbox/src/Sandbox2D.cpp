#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {
    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");
}
void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {
    // Update
    m_CameraController.OnUpdate(delta);

    // Render
    m_CameraController.OnUpdate(delta);

    zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
    zirconium::RenderCommand::Clear();

    zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());
    zirconium::Renderer2D::DrawQuad({-0.5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
    zirconium::Renderer2D::DrawQuad({0.5f, 0.6f}, {1.3f, 1.0f}, {0.8f, 0.3f, 0.2f, 1.0f});
    zirconium::Renderer2D::DrawQuad({0.8f, -0.7f}, {1.0f, 0.5f}, {0.2f, 0.8f, 0.2f, 1.0f});
    zirconium::Renderer2D::DrawTexQuad({0.0f, 0.0f, -0.1f}, {30.0f, 30.0f}, m_Texture);

    zirconium::Renderer2D::EndScene();

    // zirconium::Transform transformation = zirconium::Transform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    // m_SquareColor);
}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Scuare Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}
void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_CameraController.OnEvent(event);
}
