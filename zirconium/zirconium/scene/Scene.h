#pragma once
#include "entt.hpp"
#include "zirconium/Core/Timestep.h"

namespace zirconium {
class Scene {
public:
    Scene();
    ~Scene();

    entt::entity CreateEntity();

    // TEMP
    entt::registry& Reg() {
        return m_Registry;
    }

    void OnUpdate(TimeStep delta);

private:
    entt::registry m_Registry;
};
} // namespace zirconium
