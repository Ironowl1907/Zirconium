#pragma once

#include "zirconium.h"

#include "Panels/ContentBrowser.h"
#include "Panels/SceneHireachyPanel.h"
#include "zirconium/Renderer/EditorCamera.h"
#include <filesystem>

namespace zirconium {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() {};

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(TimeStep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& event) override;

    void NewFile();
    void OpenFile(const std::string);
    void OpenFile(const std::filesystem::path path);
    void Save();
    void SaveToFile(const std::string);

    // UI Panels
    void UI_ToolBar();

    void OnScenePlay();
    void OnSceneStop();

    void OnSimulationPlay();

    void OnDuplicateEntity();

private:
    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnMousePressed(MousePressedButtonEvent& e);

    void OnOverlayRender();

private:
    OrthoCameraController m_CameraController;

    Ref<FrameBuffer> m_Framebuffer;

    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;

    glm::vec2 m_ViewportSize = {0, 0};
    glm::vec2 m_ViewportBounds[0];

    u_int32_t m_MapWidth = 24, m_MapHeight = 13;
    // std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    std::filesystem::path m_CurrentScenePath;

    Ref<Scene> m_ActiveScene;
    Ref<Scene> m_EditorScene;

    Entity m_SquareEntity;
    Entity m_CameraEntity;
    Entity m_SecondCameraEntity;
    Entity m_HoveredEntity;

    bool m_PrimaryCamera = false;

    EditorCamera m_EditorCamera;

    bool m_ShowRenderStats = true;
    // Panels
    SceneHierarchyPanel m_SceneHierarchyPanel;
    ContentBrowserPannel m_ContentBrowserPanel;

    enum class SceneState {
        Edit = 0,
        Play = 1,
        Simulate = 2,
    };

    SceneState m_SceneState = SceneState::Edit;

    bool m_ShowPhysicsColiders = false;

    // Editor Resourses
    Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSimulate;
};

} // namespace zirconium
