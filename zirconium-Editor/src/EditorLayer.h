#pragma once

#include "ParticleSystem.h"
#include "zirconium.h"

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

private:
    OrthoCameraController m_CameraController;

    Ref<FrameBuffer> m_Framebuffer;

    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;

    glm::vec2 m_ViewportSize;

    u_int32_t m_MapWidth = 24, m_MapHeight = 13;
    std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;

    Ref<Scene> m_ActiveScene;
    Entity m_SquareEntity;
    Entity m_CameraEntity;
};
} // namespace zirconium
