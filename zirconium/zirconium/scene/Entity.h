#pragma once
#include "zrpch.h"

#include "Scene.h"
#include "core.h"
#include "entt.hpp"

namespace zirconium {
class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        ZR_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
        auto& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T& GetComponent() {
        ZR_CORE_ASSERT(HasComponent<T>(), "Entity already does not has component")
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    bool HasComponent() const {
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

    operator entt::entity() const {
        return m_EntityHandle;
    }

    operator uint64_t() const {
        return (uint64_t)m_EntityHandle;
    }

    bool operator==(const Entity& other) const {
        return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }
    bool operator!=(const Entity& other) const {
        return !operator==(other);
    }

private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;
    friend class Scene;
};
} // namespace zirconium
