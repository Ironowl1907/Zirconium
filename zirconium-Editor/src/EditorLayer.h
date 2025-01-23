#pragma once

#include "ParticleSystem.h"
#include "zirconium.h"

#include "Panels/SceneHireachyPanel.h"

namespace zirconium {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(TimeStep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event& event) override;

    void NewFile();
    void OpenFile(const std::string);
    void Save();
    void SaveToFile(const std::string);

private:
    bool OnKeyPressed(KeyPressedEvent& e);

private:
    OrthoCameraController m_CameraController;

    Ref<FrameBuffer> m_Framebuffer;

    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;

    glm::vec2 m_ViewportSize = {0, 0};

    u_int32_t m_MapWidth = 24, m_MapHeight = 13;
    std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    Ref<Scene> m_ActiveScene;
    Entity m_SquareEntity;
    Entity m_CameraEntity;
    Entity m_SecondCameraEntity;

    bool m_PrimaryCamera = false;

    // Panels
    SceneHierarchyPanel m_SceneHierarchyPanel;
};
} // namespace zirconium
