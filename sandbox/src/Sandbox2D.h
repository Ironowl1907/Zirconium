#pragma once
#include <cstdint>

#include "zirconium.h"
#include "Level.h"

class Sandbox2D : public zirconium::Layer {
public:
    Sandbox2D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(zirconium::TimeStep delta) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(zirconium::Event& event) override;

    void CreateCamera2D(const uint32_t width, const uint32_t height);

private:
    zirconium::Ref<zirconium::Texture2D> m_Texture;

    zirconium::Ref<zirconium::OrthoCamera> m_Camera;
    zirconium::Ref<Level> m_Level;
};
