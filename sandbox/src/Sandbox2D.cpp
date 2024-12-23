#include "Sandbox2D.h"
#include "Timer.h"
#include "imgui.h"
#include "zirconium.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define PROFILE_SCOPE(name) PROFILE_SCOPE_IMPL(name, TOSTRING(__LINE__))

#define PROFILE_SCOPE_IMPL(name, line)                            \
    Timer timer(name "_" line, [&](ProfileResult profileResult) { \
        m_ProfilingResults.push_back(profileResult);              \
    });

Sandbox2D::Sandbox2D()
    : Layer("SandBox2D")
    , m_CameraController(1.6f / 0.9f, true) {}

void Sandbox2D::OnAttach() {
    m_Texture = zirconium::Texture2D::Create("../sandbox/res/textures/textureTest.png");

    m_ProfilingResults = std::vector<ProfileResult>();
}
void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(zirconium::TimeStep delta) {
    PROFILE_SCOPE("Sandbox::OnUpdate");
    {
        PROFILE_SCOPE("CameraController_update");
        // Update
        m_CameraController.OnUpdate(delta);
    }
    {
        PROFILE_SCOPE("Render");
        zirconium::RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        zirconium::RenderCommand::Clear();

        zirconium::Renderer2D::BeginScene(m_CameraController.GetCamera());

        zirconium::Renderer2D::DrawQuad({-0.5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.3f, 0.9f, 1.0f});
        zirconium::Renderer2D::DrawQuad({0.5f, 0.6f}, {1.3f, 1.0f}, {0.8f, 0.3f, 0.2f, 1.0f});
        zirconium::Renderer2D::DrawQuad({0.8f, -0.7f}, {1.0f, 0.5f}, {0.2f, 0.8f, 0.2f, 1.0f});
        zirconium::Renderer2D::DrawTexQuad({0.0f, 0.0f, -0.1f}, {30.0f, 30.0f}, m_Texture);

        zirconium::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {
    for (const auto& result : m_ProfilingResults) {
        char label[50];
        snprintf(label, sizeof(label), "%.3f ms : %s", result.Time, result.Name);
        ImGui::Text("%s", label);
    }

    m_ProfilingResults.clear();
}

void Sandbox2D::OnEvent(zirconium::Event& event) {
    m_CameraController.OnEvent(event);
}
