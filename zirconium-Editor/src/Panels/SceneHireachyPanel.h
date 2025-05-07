#pragma once

#include "core.h"
#include "zirconium/Scene/Components.h"
#include "zirconium/Scene/Entity.h"
#include "zirconium/Scene/Scene.h"

namespace zirconium {

struct EntityComponentSelection {
    Entity Entity = {};
    Components Component = Components::None;
};

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    Entity GetSelectedEntity() const {
        return m_SelectionContext.Entity;
    }

    Components GetSelectedComponent() const {
        return m_SelectionContext.Component;
    }

    void SetSelectedEntity(const Entity& entity) {
        m_SelectionContext.Entity = entity;
        m_SelectionContext.Component = Components::None;
    }

    void SetContext(const Ref<Scene>& context);
    void OnImGuiRender();

private:
    void DrawEntityNode(Entity entity);
    void DrawComponents(EntityComponentSelection& selection);

private:
    Ref<Scene> m_Context;

    EntityComponentSelection m_SelectionContext;

    friend class Scene;
};
} // namespace zirconium
