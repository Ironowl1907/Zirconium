#pragma once

#include "core.h"
#include "zirconium/scene/Entity.h"
#include "zirconium/scene/Scene.h"

namespace zirconium {
class SceneHirearchyPanel {
public:
    SceneHirearchyPanel() = default;
    SceneHirearchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);
    void OnImGuiRender();

private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity ent);

private:
    Ref<Scene> m_Context;
    Entity m_SelectionContext;

    friend class Scene;
};

} // namespace zirconium
