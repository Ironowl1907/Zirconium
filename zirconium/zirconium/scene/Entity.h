#pragma once
#include "Scene.h"
#include "entt.hpp"

#include "zrpch.h"

#include "core.h"

namespace zirconium {
class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        ZR_CORE_ASSERT(!HasComponent<T>(), "Entity already has component")
        return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template <typename T>
    T& GetComponent() {
        ZR_CORE_ASSERT(HasComponent<T>(), "Entity already does not has component")
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template <typename T>
    void RemoveComponent() {
        ZR_CORE_ASSERT(HasComponent<T>(), "Entity already does not has component")
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const {
        return m_EntityHandle != entt::null;
    }

    operator entt::entity() {
        return m_EntityHandle;
    }

private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;
};
} // namespace zirconium
