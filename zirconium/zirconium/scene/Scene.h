#pragma once
#include "entt.hpp"

namespace zirconium {
class Scence {
public:
    Scence();
    ~Scence();

private:
    entt::registry m_Registry;
};
} // namespace zirconium
