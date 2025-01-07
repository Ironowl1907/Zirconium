#pragma once

#include "zirconium.h"
#include "ParticleSystem.h"

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

    // This shouldn't be here TEMP
    Ref<VertexArray> m_VertexArray;
    Ref<Shader> m_FlatColorShader;
    Ref<FrameBuffer> m_Framebuffer;

    Ref<Texture2D> m_Texture;
    Ref<Texture2D> m_SpriteSheet;

    ParticleSystem m_ParticleSystem;
    ParticleProps m_Particle;

    u_int32_t m_MapWidth = 24, m_MapHeight = 13;
    std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
};
} // namespace zirconium
