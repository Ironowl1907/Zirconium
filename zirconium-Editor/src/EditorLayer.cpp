#include "Panels/SceneHireachyPanel.h"
#include "zrpch.h"

#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "zirconium.h"

#include "zirconium/Utils/PlatformUtils.h"
#include "zirconium/scene/SceneSerializer.h"
#include <cstdint>

namespace zirconium {

class CameraController : public ScriptableEntity {
public:
    void OnCreate() {
        auto& transform = GetComponent<TransformComponent>();
        transform.Translation.x = rand() % 10 - 5.0f;
    }
    void OnDestroy() {}

    void OnUpdate(TimeStep delta) {
        float speed = 5.0f;
        auto& tc = GetComponent<TransformComponent>();
        if (Input::IsKeyPressed(ZR_KEY_A))
            tc.Translation.x -= speed * delta;
        if (Input::IsKeyPressed(ZR_KEY_W))
            tc.Translation.y += speed * delta;
        if (Input::IsKeyPressed(ZR_KEY_D))
            tc.Translation.x += speed * delta;
        if (Input::IsKeyPressed(ZR_KEY_S))
            tc.Translation.y -= speed * delta;
    }
};

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
    , m_CameraController(1.6f / 0.9f, true) {}

void EditorLayer::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_Texture = Texture2D::Create("../sandbox/res/textures/textureTest.png");

    m_CameraController.SetZoomLevel(5.0f);

    FrameBufferSpecification fbSpec;
    fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8,
                          FramebufferTextureFormat::Depth};
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = FrameBuffer::Create(fbSpec);

    // Scene
    m_ActiveScene = std::make_shared<Scene>();

    m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1, 1000.0f);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}
void EditorLayer::OnDetach() {}

void EditorLayer::OnUpdate(TimeStep delta) {

    ZR_PROFILE_FUNCTION();

    FrameBufferSpecification spec = m_Framebuffer->GetSpecification();

    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
        m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    if (m_ViewportFocused) {
        m_CameraController.OnUpdate(delta); // Deprecated
    }
    // Update Editor Camera
    m_EditorCamera.OnUpdate(delta);

    {
        ZR_PROFILE_SCOPE("Render");
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.1804, 0.1804, 0.1804, 1}); // Set clear color (dark gray)
        RenderCommand::Clear();

        // Update Scene
        m_ActiveScene->OnUpdateEditor(delta, m_EditorCamera);

        m_Framebuffer->Unbind();
    }
}

static bool s_Opening = false;
static bool s_SavingTo = false;
static std::string s_FilePath = "";

void EditorLayer::OnImGuiRender() {

    ZR_PROFILE_FUNCTION();

    // bool t = true;
    // ImGui::ShowDemoWindow(&t);

    // READ THIS !!!
    // TL;DR; this demo is more complicated than what most users you would normally use.
    // If we remove all options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    //     }
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen ==
    // false)
    // - (2) we allow the host window to have padding (when opt_padding == true)
    // - (3) we expose many flags and need a way to have them visible.
    // - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
    //      in your code, but we don't here because we allow the window to be floating)

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    } else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSize = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSize;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.

            if (ImGui::MenuItem("New", "Ctrl+N")) {
                NewFile();
            }
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                s_Opening = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                Save();
            }
            if (ImGui::MenuItem("Save to...", "Ctrl+Shift+S")) {
                s_SavingTo = true;
            }

            if (ImGui::MenuItem("Exit"))
                Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Renderer 2D stats: ");
        ImGui::Text("Draw call %d", stats.DrawCalls);
        ImGui::Text("Quads %d", stats.QuadCount);
        ImGui::Text("Vertices %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices %d", stats.GetTotalIndexCount());

        ImGui::End();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    ImVec2 sizeViewport = ImGui::GetContentRegionAvail();
    if (sizeViewport.x > 0 && sizeViewport.y > 0) {
        if (sizeViewport.x != m_ViewportSize.x || sizeViewport.y != m_ViewportSize.y) {
            m_ViewportSize = {sizeViewport.x, sizeViewport.y};
            m_CameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
    } else {
        ZR_CORE_WARN("Invalid Viewport Size: ({}, {})", sizeViewport.x, sizeViewport.y);
    }
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(1);
    ImGui::Image(textureID, {m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();

    if (s_Opening) {
        if (FileDialogs::OpenFile(s_FilePath)) {
            OpenFile(s_FilePath);
            s_Opening = false;
        }
    }
    if (s_SavingTo) {
        if (FileDialogs::SaveFile(s_FilePath)) {
            SaveToFile(s_FilePath);
            s_SavingTo = false;
        }
    }
}

void EditorLayer::NewFile() {
    ZR_CORE_WARN("NewFile");
    m_ActiveScene = std::make_shared<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}
void EditorLayer::OpenFile(const std::string path) {
    ZR_CORE_WARN("OpenFile: {}", path);
    if (!path.empty()) {
        m_ActiveScene = std::make_shared<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize(path);
    }
}
void EditorLayer::Save() {
    ZR_CORE_WARN("Save");
}
void EditorLayer::SaveToFile(const std::string path) {
    ZR_CORE_WARN("SaveToFile: {}", path);
    if (!path.empty()) {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(path);
    }
}

void EditorLayer::OnEvent(Event& event) {
    m_EditorCamera.OnEvent(event);
    m_CameraController.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ZR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
    // Shortcuts
    if (e.GetRepeatedCount() > 0)
        return false;

    bool control = Input::IsKeyPressed(ZR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(ZR_KEY_RIGHT_CONTROL);
    bool shift = Input::IsKeyPressed(ZR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(ZR_KEY_RIGHT_SHIFT);

    switch (e.GetKeyCode()) {
    case ZR_KEY_S: {
        if (control && shift) {
            s_SavingTo = true;
        }
        break;
    }

    case ZR_KEY_O: {
        if (control) {
            s_Opening = true;
        }
        break;
    }

    case ZR_KEY_N: {
        if (control) {
            NewFile();
        }
        break;
    }

    default:
        break;
    }
    return true;
}
} // namespace zirconium
