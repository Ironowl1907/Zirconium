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

private:
    entt::registry m_Registry;

    friend class Entity;
};
} // namespace zirconium
