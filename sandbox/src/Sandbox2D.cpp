#include "Sandbox2D.h"
#include "Level.h"
#include "imgui.h"
#include "zirconium.h"
#include <memory>

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D") {
    auto& window = zirconium::Application::Get().GetWindow();
    m_Camera = std::make_shared<zirconium::OrthoCamera>(0, 0, 0, 0);
    CreateCamera2D(window.GetWidth(), window.GetHeight());
}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Level = std::make_shared<Level>();
}
void Sandbox2D::OnDetach() {}
void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    // Update
    {
        ZR_PROFILE_SCOPE("Render");
        // zirconium::RenderCommand::SetClearColor({0.5294, 0.8078, 0.9216, 1.0}); // Set clear color (sky blue)
        // zirconium::RenderCommand::Clear();

        // zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        // {
        //     ZR_PROFILE_SCOPE("Drawing");
        //     zirconium::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
        // }

        // zirconium::Renderer2D::EndScene();

        zirconium::RenderCommand::SetClearColor({0.5294, 0.8078, 0.9216, 1.0}); // Set clear color (sky blue)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer2D::BeginScene(*m_Camera);

        m_Level->OnUpdate(delta);
        m_Level->OnRender();

        zirconium::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {
  m_Level->GetPlayer().OnImGuiRender();
}

void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_Level->OnEvent();
}

void Sandbox2D::CreateCamera2D(const uint32_t width, const uint32_t height) {
    float aspectRatio = (float)width / (float)height;

    float heightSize = 10.0f;
    float up, down, right, left;
    up = heightSize;
    down = -heightSize;
    right = heightSize * aspectRatio;
    left = -heightSize * aspectRatio;

    m_Camera->SetProyection(left, right, up, down);
}
