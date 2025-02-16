#pragma once
#include "SceneSerializer.h"
#include "entt.hpp"
#include "zirconium/Core/Timestep.h"

#include "zirconium/Renderer/EditorCamera.h"
#include <cstddef>

class b2World;

namespace zirconium {
class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = "");

    void OnRuntimeStart();
    void OnRuntimeStop();

    void OnUpdateEditor(TimeStep delta, EditorCamera& camera);
    void OnUpdateRuntime(TimeStep delta);
    void OnViewportResize(const uint32_t& width, const uint32_t& height);
    void DeleteEntity(Entity entity);

private:
    template <typename T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_Registry;

    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;


    friend class Entity;
    friend class SceneSerializer;
    friend class SceneHierarchyPanel;
};
} // namespace zirconium
