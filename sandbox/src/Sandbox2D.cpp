#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");
}
void Sandbox2D::OnDetach() {}
void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(delta);
    {
        ZR_PROFILE_SCOPE("Render");
        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        {
            ZR_PROFILE_SCOPE("Drawing");
            zirconium::Renderer2D::DrawQuad({-0.5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
            zirconium::Renderer2D::DrawQuad({0.5f, 0.6f}, {1.3f, 1.0f}, {0.8f, 0.3f, 0.2f, 1.0f});
            zirconium::Renderer2D::DrawQuad({0.8f, -0.7f}, {1.0f, 0.5f}, {0.2f, 0.8f, 0.2f, 1.0f});
            // zirconium::Renderer2D::DrawRotatedQuad({-0.8f, 0.7f, 0.1f}, {1.0f, 1.0f}, 45.0f, {0.5f, 0.1f, 0.8f, 1.0f});
            // zirconium::Renderer2D::DrawRotatedTexQuad({0.0f, 0.0f, -0.1f}, {30.0f, 30.0f}, 45.0f, m_Texture,10.0f);
        }

        zirconium::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {}

void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_CameraController.OnEvent(event);
}
