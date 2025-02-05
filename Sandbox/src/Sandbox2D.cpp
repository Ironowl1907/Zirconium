#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");

    m_CameraController.SetZoomLevel(5.0f);
}
void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    // Reset Stats
    zirconium::Renderer2D::ResetStats();
    m_CameraController.OnUpdate(delta);
    {
        ZR_PROFILE_SCOPE("Render");
        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        // zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());
        // for (int y = 0; y < 100; y++) {
        //     for (int x = 0; x < 100; x++) {
        //         zirconium::Renderer2D::DrawQuad({x - m_CameraController.GetBounds().GetWidth() / 2, y}, {0.9f, 0.9f},
        //                                         {x / 100.0f, y / 100.0f, 1.0f, 1.0f});
        //     }
        // }
        // zirconium::Renderer2D::EndScene();
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
