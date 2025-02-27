#pragma once
#include "SceneSerializer.h"
#include "entt.hpp"
#include "zirconium/Core/Timestep.h"

#include "zirconium/Renderer/EditorCamera.h"
#include <cstddef>

struct b2WorldId;

namespace zirconium {
class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    static Ref<Scene> Copy(Ref<Scene> other);

    Entity CreateEntity(const std::string& name = "");
    Entity CreateEntityWithID(const uint64_t id, const std::string& name);

    void OnRuntimeStart();
    void OnRuntimeStop();

    void DuplicateEntity(Entity entity);

    Entity GetMainCameraEntity();

    void OnUpdateEditor(TimeStep delta, EditorCamera& camera);
    void OnUpdateRuntime(TimeStep delta);
    void OnViewportResize(const uint32_t& width, const uint32_t& height);
    void DeleteEntity(Entity entity);

    template <typename... Components>
    auto GetAllEntitiesWith() {
        return m_Registry.view<Components...>();
    }

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_Registry;

    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    b2WorldId* m_WorldID;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
};
} // namespace zirconium
