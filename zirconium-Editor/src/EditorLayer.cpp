#include "Panels/SceneHireachyPanel.h"
#include "zrpch.h"

#include "EditorLayer.h"
#include "imgui.h"
#include "zirconium.h"

#include "zirconium/Utils/PlatformUtils.h"
#include "zirconium/scene/SceneSerializer.h"
#include <cstdint>
#include <filesystem>
#include <glm/ext/matrix_transform.hpp>
#include <stdexcept>

namespace zirconium {

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
    , m_CameraController(1.6f / 0.9f, true) {}

void EditorLayer::OnAttach() {

    ZR_PROFILE_FUNCTION();

    m_CameraController.SetZoomLevel(5.0f);

    FrameBufferSpecification fbSpec;
    fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
                          FramebufferTextureFormat::Depth};
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = FrameBuffer::Create(fbSpec);

    // Scene
    m_ActiveScene = std::make_shared<Scene>();

    m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1, 1000.0f);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    ZR_CORE_TRACE("Current working path: {}", std::filesystem::current_path().string());

    m_IconPlay = Texture2D::Create("zirconium-Editor/res/editorImg/PlayButton.png");
    m_IconStop = Texture2D::Create("zirconium-Editor/res/editorImg/StopButton.png");
    m_IconSimulate = Texture2D::Create("zirconium-Editor/res/editorImg/SimulateButton.png");
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

        // Set out entity ID attachment to -1
        m_Framebuffer->ClearAttachment(1, 0);

        // Update Scene
        switch (m_SceneState) {
        case SceneState::Play: {
            m_ActiveScene->OnUpdateRuntime(delta);
            break;
        }
        case SceneState::Edit: {
            m_ActiveScene->OnUpdateEditor(delta, m_EditorCamera);
            m_EditorScene = m_ActiveScene;
            break;
        }
        case SceneState::Simulate: {
            m_ActiveScene->OnUpdateSimulation(delta, m_EditorCamera);
            break;
        }
        }

        OnOverlayRender();

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = m_ViewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        int pixelData;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY) - 1;
            // ZR_CORE_WARN(pixelData);
            m_HoveredEntity =
                (pixelData == -1) ? Entity(entt::null, nullptr) : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

        m_Framebuffer->Unbind();
    }
}

void EditorLayer::OnOverlayRender() {

    if (m_SceneState == SceneState::Play) {
        Entity camera = m_ActiveScene->GetMainCameraEntity();
        Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera,
                               camera.GetComponent<TransformComponent>().GetTransform());
    } else {
        Renderer2D::BeginScene(m_EditorCamera);
    }

    // Show Selected Enity
    if (Entity selected = m_SceneHierarchyPanel.GetSelectedEntity()) {
        if (selected.HasComponent<TransformComponent>()) {
            auto transform = selected.GetComponent<TransformComponent>().GetTransform();

            // Draw Red quad
            Renderer2D::DrawRect(transform, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), -1);
        }
    }

    if (m_ShowPhysicsColiders) {
        // Circle Coliders
        {
            auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleColiderComponent>();
            for (auto entity : view) {
                auto [tc, cc2D] = view.get<TransformComponent, CircleColiderComponent>(entity);

                glm::vec3 translation = tc.Translation + glm::vec3(cc2D.Offset, 0.001f);
                glm::vec3 scale = glm::vec3(2.0f) * tc.Scale * glm::vec3(cc2D.Radius);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);
                Renderer2D::DrawCircle(transform, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), -1, 0.02f);
                // Renderer2D::DrawTransformedQuad(transform, glm::vec4(1.0f));
                // Renderer2D::DrawTransformedQuad(glm::mat4(1.0f), glm::vec4(1.0f));
            }
        }

        // BoxColiders
        {
            auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxColiderComponent>();
            for (auto entity : view) {
                auto [tc, bc2D] = view.get<TransformComponent, BoxColiderComponent>(entity);

                glm::vec3 translation = tc.Translation + glm::vec3(bc2D.Offset, 0.001f);
                glm::vec3 scale = glm::vec3(bc2D.Size * 2.0f, 0.001f) * tc.Scale;

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) *
                                      glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
                                      glm::scale(glm::mat4(1.0f), scale);
                Renderer2D::DrawRect(transform, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
                // Renderer2D::DrawTransformedQuad(transform, glm::vec4(1.0f));
                // Renderer2D::DrawTransformedQuad(glm::mat4(1.0f), glm::vec4(1.0f));
            }
        }
    }

    Renderer2D::EndScene();
}

void EditorLayer::OnScenePlay() {
    if (m_SceneState == SceneState::Simulate) {
        ZR_CORE_WARN("Hey");
        OnSceneStop();
    }

    m_SceneState = SceneState::Play;

    ZR_ASSERT(m_EditorScene, "");
    m_ActiveScene = Scene::Copy(m_EditorScene);

    m_ActiveScene->OnRuntimeStart();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}
void EditorLayer::OnSceneStop() {
    m_SceneState = SceneState::Edit;
    m_ActiveScene->OnRuntimeStop();

    m_ActiveScene = m_EditorScene;
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OnSimulationPlay() {
    if (m_SceneState == SceneState::Play) {
        ZR_CORE_WARN("Hey");
        OnSceneStop();
    }

    m_SceneState = SceneState::Simulate;

    ZR_ASSERT(m_EditorScene, "");

    m_ActiveScene = Scene::Copy(m_EditorScene);

    m_ActiveScene->OnSimulationStart();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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
        m_ContentBrowserPanel.OnImGuiRender();

        if (m_ShowRenderStats) {
            static int location = 3;
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            if (location >= 0) {
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
                window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
                window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
                window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(viewport->ID);
                window_flags |= ImGuiWindowFlags_NoMove;
            } else if (location == -2) {
                // Center window
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                window_flags |= ImGuiWindowFlags_NoMove;
            }
            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
            if (ImGui::Begin("Stats", &m_ShowRenderStats, window_flags)) {
                ImGui::Text("Renderer Stats\n"
                            "(right-click to change position)");
                ImGui::Separator();
                auto stats = Renderer2D::GetStats();
                ImGui::Text("Draw call %d", stats.DrawCalls);
                ImGui::Text("Quads %d", stats.QuadCount);
                ImGui::Text("Vertices %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices %d", stats.GetTotalIndexCount());
                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::MenuItem("Custom", NULL, location == -1))
                        location = -1;
                    if (ImGui::MenuItem("Center", NULL, location == -2))
                        location = -2;
                    if (ImGui::MenuItem("Top-left", NULL, location == 0))
                        location = 0;
                    if (ImGui::MenuItem("Top-right", NULL, location == 1))
                        location = 1;
                    if (ImGui::MenuItem("Bottom-left", NULL, location == 2))
                        location = 2;
                    if (ImGui::MenuItem("Bottom-right", NULL, location == 3))
                        location = 3;
                    if (m_ShowRenderStats && ImGui::MenuItem("Close"))
                        m_ShowRenderStats = false;
                    ImGui::EndPopup();
                }
            }

            ImGui::End();

            ImGui::Begin("Settings");
            ImGui::Checkbox("Show Physics Colides", &m_ShowPhysicsColiders);
            ImGui::End();
        }
    } // namespace zirconium

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
    ImGui::Begin("Viewport");
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();

    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

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
    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
    ImGui::Image(textureID, {m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

    if (ImGui::BeginDragDropTarget()) {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
            const char* data = reinterpret_cast<const char*>(payload->Data);
            const int dataSize = payload->DataSize;
            try {
                OpenFile(std::string(data));
            } catch (const std::runtime_error& e) {
                ZR_CORE_ERROR("Coundn't open file! {}", e.what());
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    UI_ToolBar();

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

void EditorLayer::UI_ToolBar() {

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 2.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{0.0f, 0.0f});

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.0f, 0.0f, 0.0f, 0.0f});

    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f});
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{buttonActive.x, buttonActive.y, buttonActive.z, 0.5f});

    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 4.0f;
    {
        Ref<Texture2D> icon =
            (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x * .5f - (size * .5f));
        if (ImGui::ImageButton("##playbutton", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0),
                               ImVec2(1, 1))) {
            if (m_SceneState == SceneState::Play)
                OnSceneStop();
            else if (m_SceneState == SceneState::Edit)
                OnScenePlay();
            else {
                OnSceneStop();
                OnScenePlay();
            }
        }
    }
    ImGui::SameLine();
    {
        Ref<Texture2D> icon =
            (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
        // ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x * .5f - (size * .5f));
        if (ImGui::ImageButton("##simulatebutton", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0),
                               ImVec2(1, 1))) {
            if (m_SceneState == SceneState::Simulate)
                OnSceneStop();
            else if (m_SceneState == SceneState::Edit)
                OnSimulationPlay();
            else {
                OnSceneStop();
                OnSimulationPlay();
            }
        }
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void EditorLayer::NewFile() {
    ZR_CORE_WARN("NewFile");
    m_ActiveScene = std::make_shared<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    m_CurrentScenePath = std::filesystem::path();
}
void EditorLayer::OpenFile(const std::string path) {

    if (m_SceneState != SceneState::Edit)
        m_SceneState = SceneState::Edit;

    ZR_CORE_WARN("OpenFile: {}", path);
    if (path.empty())
        return;

    Ref<Scene> newScene = std::make_shared<Scene>();
    SceneSerializer serializer(newScene);

    if (serializer.Deserialize(path)) {
        m_EditorScene = newScene;
        m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_EditorScene);

        m_ActiveScene = m_EditorScene;
    }
    Renderer2D::ClearVB();
    m_CurrentScenePath = path;
}

void EditorLayer::OpenFile(const std::filesystem::path path) {
    OpenFile(path.string());
}

void EditorLayer::Save() {
    if (!m_CurrentScenePath.empty())
        SaveToFile(m_CurrentScenePath);
}
void EditorLayer::SaveToFile(const std::string path) {
    ZR_CORE_WARN("SaveToFile: {}", path);
    if (!path.empty()) {
        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(path);
    }

    m_CurrentScenePath = path;
}

void EditorLayer::OnEvent(Event& event) {
    m_EditorCamera.OnEvent(event);
    m_CameraController.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ZR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MousePressedButtonEvent>(ZR_BIND_EVENT_FN(EditorLayer::OnMousePressed));
}

bool EditorLayer::OnMousePressed(MousePressedButtonEvent& e) {
    if (e.GetMouseButton() == ZR_MOUSE_BUTTON_LEFT) {
        if (m_ViewportHovered && !Input::IsKeyPressed(ZR_KEY_LEFT_ALT))
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
    }
    return false;
}

void EditorLayer::OnDuplicateEntity() {
    if (m_SceneState != EditorLayer::SceneState::Edit)
        return;
    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selectedEntity) {
        m_EditorScene->DuplicateEntity(selectedEntity);
    }
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
    // Shortcuts
    if (e.GetRepeatedCount() > 0)
        return false;

    bool control = Input::IsKeyPressed(ZR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(ZR_KEY_RIGHT_CONTROL);
    bool shift = Input::IsKeyPressed(ZR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(ZR_KEY_RIGHT_SHIFT);

    switch (e.GetKeyCode()) {
    case ZR_KEY_S: {
        if (control) {
            if (shift)
                s_SavingTo = true;
            else
                Save();
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

    case ZR_KEY_D: {
        if (control) {
            OnDuplicateEntity();
        }
        break;
    }

    default:
        break;
    }
    return true;
}

} // namespace zirconium
