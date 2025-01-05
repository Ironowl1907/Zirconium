#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");
    m_SpriteSheet = zirconium::Texture2D::Create("../sandbox/res/textures/RPGpack_sheet_2X.png");
    m_Stairs = zirconium::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7, 6}, {128, 128});
    m_Barrel = zirconium::SubTexture2D::CreateFromCoords(m_SpriteSheet, {9, 2}, {128, 128});
    m_Tree = zirconium::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2, 1}, {128, 128}, {1, 2});

    m_Particle.ColorBegin = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};
    m_Particle.ColorEnd = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    m_Particle.SizeBegin = 0.3f, m_Particle.SizeVariation = 0.1f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime = 10.0f;
    m_Particle.Velocity = {0.0f, 0.0f};
    m_Particle.Aceleration = {3.0f, 1.0f};
    m_Particle.Position = {0.0f, 0.0f};
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

        // zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());
        // m_ParticleSystem.OnUpdate(delta);
        // m_ParticleSystem.OnRender(m_CameraController.GetCamera());
        // zirconium::Renderer2D::EndScene();

        // zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        // {
        //     static float rotation = 0.0f;
        //     rotation += 200.0f * delta;
        //     ZR_PROFILE_SCOPE("Drawing");
        //     zirconium::Renderer2D::DrawRotatedTexQuad({10.0f, 10.0f}, {1.0f, 1.0f}, 45.0f, m_Texture, 50.0f);
        //     zirconium::Renderer2D::DrawQuad({-0.5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
        //     zirconium::Renderer2D::DrawQuad({0.5f, 0.6f}, {1.3f, 1.0f}, {0.8f, 0.3f, 0.2f, 1.0f});
        //     zirconium::Renderer2D::DrawQuad({0.8f, -0.7f}, {1.0f, 0.5f}, {0.2f, 0.8f, 0.2f, 1.0f});
        //     zirconium::Renderer2D::DrawRotatedQuad({1.8f, -1.7f}, {1.0f, 0.5f}, glm::radians(rotation),
        //                                            {0.2f, 0.7f, 0.8f, 1.0f});
        //     zirconium::Renderer2D::DrawTexQuad({0.0f, 0.0f}, {10.0f, 10.0f}, m_Texture, 10.0f);
        // }

        // zirconium::Renderer2D::EndScene();
        // zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        // {
        //     ZR_PROFILE_SCOPE("Drawing");
        //     for (float y = -20.0f; y < 20.0f; y += 0.1f) {
        //         for (float x = -20.0f; x < 20.0f; x += 0.1f) {
        //             glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.4f};
        //             zirconium::Renderer2D::DrawQuad({x, y, 1.0f}, {0.45f, 0.45f}, color);
        //         }
        //     }
        // }

        // zirconium::Renderer2D::EndScene();

        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());
        zirconium::Renderer2D::DrawTexQuad({0.5f, 0.0f}, {1.0f, 1.0f}, m_Stairs, 1.0f);
        zirconium::Renderer2D::DrawTexQuad({-0.5f, 0.0f}, {1.0f, 1.0f}, m_Barrel, 1.0f);
        zirconium::Renderer2D::DrawTexQuad({2.0f, 0.0f}, {1.0f, 2.0f}, m_Tree, 1.0f);
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
