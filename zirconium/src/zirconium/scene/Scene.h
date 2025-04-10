#pragma once
#include "SceneSerializer.h"
#include "entt.hpp"
#include "zirconium/Core/Timestep.h"

#include "sol/sol.hpp"
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

    void OnSimulationStart();
    void OnSimulationStop();
    bool OnRuntimeStart();
    void OnRuntimeStop();

    void OnUpdateSimulation(TimeStep delta, EditorCamera& camera);
    void OnUpdateEditor(TimeStep delta, EditorCamera& camera);
    void OnUpdateRuntime(TimeStep delta);

    void OnViewportResize(const uint32_t& width, const uint32_t& height);

    void DeleteEntity(Entity entity);


    void DuplicateEntity(Entity entity);
    Entity GetMainCameraEntity();

    template <typename... Components>
    auto GetAllEntitiesWith() {
        return m_Registry.view<Components...>();
    }

    void RenderScene(EditorCamera& camera);

    entt::registry& GetRegistry() {
        return m_Registry;
    }

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);
    void OnPhysicsInit();
    bool OnScriptsInit();
    void OnPhysicsShutdown();

private:
    entt::registry m_Registry;

    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    b2WorldId* m_WorldID;

    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
    friend class ScriptingSystem;
};
} // namespace zirconium
