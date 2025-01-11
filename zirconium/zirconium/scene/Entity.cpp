#include "zrpch.h"

#include "Entity.h"
namespace zirconium {

Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle)
    , m_Scene(scene) {}
} // namespace zirconium
