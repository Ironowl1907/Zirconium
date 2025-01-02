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
    // Reset Stats
    zirconium::Renderer2D::ResetStats();
    m_CameraController.OnUpdate(delta);
    {
        ZR_PROFILE_SCOPE("Render");
        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        {
            static float rotation = 0.0f;
            rotation += 200.0f * delta;
            ZR_PROFILE_SCOPE("Drawing");
            zirconium::Renderer2D::DrawRotatedTexQuad({10.0f, 10.0f}, {1.0f, 1.0f}, 45.0f, m_Texture, 50.0f);
            zirconium::Renderer2D::DrawQuad({-0.5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
            zirconium::Renderer2D::DrawQuad({0.5f, 0.6f}, {1.3f, 1.0f}, {0.8f, 0.3f, 0.2f, 1.0f});
            zirconium::Renderer2D::DrawQuad({0.8f, -0.7f}, {1.0f, 0.5f}, {0.2f, 0.8f, 0.2f, 1.0f});
            zirconium::Renderer2D::DrawRotatedQuad({1.8f, -1.7f}, {1.0f, 0.5f}, rotation, {0.2f, 0.7f, 0.8f, 1.0f});
            zirconium::Renderer2D::DrawTexQuad({0.0f, 0.0f}, {10.0f, 10.0f}, m_Texture, 10.0f);
        }

        zirconium::Renderer2D::EndScene();
        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        {
            ZR_PROFILE_SCOPE("Drawing");
            for (float y = -5.0f; y < 5.0f; y += 0.1f) {
                for (float x = 5.0f; x < 5.0f; x += 0.1f) {
                    glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.4f};
                    zirconium::Renderer2D::DrawQuad({x, y, 1.0f}, {0.45f, 0.45f}, color);
                }
            }
        }

        zirconium::Renderer2D::EndScene();
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
