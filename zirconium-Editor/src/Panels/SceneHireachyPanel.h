#pragma once

#include "core.h"
#include "zirconium/Scene/Entity.h"
#include "zirconium/Scene/Scene.h"
#include "zirconium/Scene/Components.h"

namespace zirconium {
class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    Entity GetSelectedEntity() const {
        return m_SelectionContext.Entity;
    }
    void SetSelectedEntity(const Entity& entity) {
        m_SelectionContext.Entity = entity;
        m_SelectionContext.Component = Components::None;
    }

    void SetContext(const Ref<Scene>& context);
    void OnImGuiRender();

private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(Entity ent);

private:
    Ref<Scene> m_Context;

    struct EntityComponentSelection {
        Entity Entity = {};
        Components Component = Components::None;
    };

    EntityComponentSelection m_SelectionContext;

    friend class Scene;
};

} // namespace zirconium
