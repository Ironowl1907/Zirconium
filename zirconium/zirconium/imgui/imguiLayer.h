#pragma once

#include "../events/ApplicationEvent.h"
#include "../events/KeyEvent.h"
#include "../events/MouseEvent.h"
#include "../layers/Layer.h"

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
    float m_time = 0.0f;
};

} // namespace zirconium
