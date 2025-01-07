#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"
#include <cstdint>

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");

    m_Particle.ColorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
    m_Particle.ColorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    m_Particle.SizeBegin = 0.3f, m_Particle.SizeVariation = 0.1f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 10.0f;
    m_Particle.Velocity = {0.0f, 0.0f};
    m_Particle.Aceleration = {3.0f, 1.0f};
    m_Particle.Position = {0.0f, 0.0f};

    m_CameraController.SetZoomLevel(5.0f);
}
void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    // Update

    // Reset Stats
    zirconium::Renderer2D::ResetStats();
    m_CameraController.OnUpdate(delta);
    {
        ZR_PROFILE_SCOPE("Render");
        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (int y = 0; y < 100; y++) {
            for (int x = 0; x < 100; x++) {
                zirconium::Renderer2D::DrawQuad({x - m_CameraController.GetBounds().GetWidth() / 2, y}, {0.9f, 0.9f},
                                                {x / 100.0f, y / 100.0f, 1.0f, 1.0f});
            }
        }
        zirconium::Renderer2D::EndScene();
    }

    if (zirconium::Input::IsMouseButtonPressed(ZR_MOUSE_BUTTON_LEFT)) {
        auto [x, y] = zirconium::Input::GetMousePosition();
        auto width = zirconium::Application::Get().GetWindow().GetWidth();
        auto height = zirconium::Application::Get().GetWindow().GetHeight();

        auto bounds = m_CameraController.GetBounds();
        auto pos = m_CameraController.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_Particle.Position = {x + pos.x, y + pos.y};
        for (int i = 0; i < 50; i++) {
            m_ParticleSystem.Emit(m_Particle);
        }
    }
}

void Sandbox2D::OnImGuiRender() {

    ZR_PROFILE_FUNCTION();

    ImGui::Begin("Profiling");

    auto stats = zirconium::Renderer2D::GetStats();
    ImGui::Text("Renderer 2D stats: ");
    ImGui::Text("Draw call %d", stats.DrawCalls);
    ImGui::Text("Quads %d", stats.QuadCount);
    ImGui::Text("Vertices %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices %d", stats.GetTotalIndexCount());

    ImGui::End();
}

void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_CameraController.OnEvent(event);
}
