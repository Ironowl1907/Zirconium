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

    static void Begin();
    static void End();

private:
    // float m_time = 0.0f;
};

} // namespace zirconium
