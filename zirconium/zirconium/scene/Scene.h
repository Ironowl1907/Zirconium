#pragma once
#include "entt.hpp"
#include "zirconium/Core/Timestep.h"

namespace zirconium {
class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = "");

    void OnUpdate(TimeStep delta);
    void OnViewportResize(const uint32_t& width, const uint32_t& height);

private:
    entt::registry m_Registry;

    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;

    friend class Entity;
};
} // namespace zirconium
