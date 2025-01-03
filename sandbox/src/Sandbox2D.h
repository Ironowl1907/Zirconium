#pragma once

#include "zirconium.h"

class Sandbox2D : public zirconium::Layer {
public:
    Sandbox2D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(zirconium::TimeStep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(zirconium::Event& event) override;

private:
    zirconium::OrthoCameraController m_CameraController;

    // This shouldn't be here TEMP
    zirconium::Ref<zirconium::VertexArray> m_VertexArray;
    zirconium::Ref<zirconium::Shader> m_FlatColorShader;

    zirconium::Ref<zirconium::Texture2D> m_Texture;

};
