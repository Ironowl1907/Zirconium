#pragma once
#include "zrpch.h"

#include "Components.h"
#include "Scene.h"
#include "core.h"
#include "entt.hpp"
#include "zirconium/Core/UUID.h"
#include <cstdint>

namespace zirconium {
class Entity {
public:
    Entity();
    Entity(entt::entity handle, Scene* scene);

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args) {
        ZR_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
        auto& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T, typename... Args>
    T& AddOrReplaceComponent(Args&&... args) {
        auto& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template <typename T>
    T& GetComponent() const {
        ZR_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component")
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

    uint64_t GetID() const {
        return GetComponent<IDComponent>().ID;
    }

    const std::string& GetTag() const {
        return GetComponent<TagComponent>().Tag;
    }

private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;
    friend class Scene;
};
} // namespace zirconium
