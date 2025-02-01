#pragma once

#include "core.h"
#include "zirconium/scene/Entity.h"
#include "zirconium/scene/Scene.h"

namespace zirconium {
class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    Entity GetSelectedEntity() const {
        return m_SelectionContext;
    }
    void SetSelectedEntity(const Entity& entity) {
        m_SelectionContext = entity;
    }

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
