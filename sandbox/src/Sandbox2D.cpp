#include "Sandbox2D.h"
#include "imgui.h"
#include "zirconium.h"
#include <cstdint>

static const char* s_MapTiles = //
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWDDDDDDDDDWWWWW"
    "WWWWDDDDDDDDDDDDDDDDWWWW"
    "WWWWDDDDDDDDDDDDDDDDWWWW"
    "WWWWDDDDDDDDDDDDWWWWWWWW"
    "WWWWWDDDDDDDDDDDDDDDWWWW"
    "WWWWDDDDDDDDDDDDDDDDWWWW"
    "WWWWDDDDDDDDDDDDDDDDWWWW"
    "WWWWDDDDDDDDDDDDDDDDWWWW"
    "WWWWWWWWDDDDDDDDDDDWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");
    m_SpriteSheet = zirconium::Texture2D::Create("../sandbox/res/textures/RPGpack_sheet_2X.png");

    s_TextureMap['D'] = zirconium::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11}, {128, 128});
    s_TextureMap['W'] = zirconium::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128});

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

        for (uint32_t y = 0; y < m_MapHeight; y++) {
            for (uint32_t x = 0; x < m_MapWidth; x++) {
                char tileType = s_MapTiles[x + y * m_MapWidth];
                zirconium::Ref<zirconium::SubTexture2D> texture;
                if (s_TextureMap.find(tileType) != s_TextureMap.end())
                    texture = s_TextureMap[tileType];
                else
                    ZR_ASSERT(false, "Undefined tiletype '{}'", tileType);
                zirconium::Renderer2D::DrawTexQuad({x, y},{1.0f, 1.0f}, texture);
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
