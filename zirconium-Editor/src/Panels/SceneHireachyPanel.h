#pragma once

#include "core.h"
#include "zirconium/scene/Scene.h"
#include "zirconium/scene/Entity.h"


namespace zirconium {
class SceneHirearchyPanel {
public:
    SceneHirearchyPanel() = default;
    SceneHirearchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);
    void OnImGuiRender();

private:
    Ref<Scene> m_Context;

    friend class Scene;
};

} // namespace zirconium
