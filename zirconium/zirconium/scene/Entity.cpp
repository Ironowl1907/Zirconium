#include "zrpch.h"

#include "Entity.h"
namespace zirconium {

Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle)
    , m_Scene(scene) {}

Entity::Entity()
    : m_EntityHandle(entt::null)
    , m_Scene(nullptr) {}

} // namespace zirconium
