#pragma once

#include "zirconium/Events/ApplicationEvent.h"
#include "zirconium/Events/KeyEvent.h"
#include "zirconium/Events/MouseEvent.h"
#include "zirconium/Layers/Layer.h"

namespace zirconium {
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    virtual void OnEvent(Event& e) override;

    static void Begin();
    static void End();

    void BlockEvents(bool block) {
        m_BlockEvents = block;
    }

    void SetDarkThemeColors();

private:
    bool m_BlockEvents = true;
    // float m_Time = 0.0f;
};

} // namespace zirconium
