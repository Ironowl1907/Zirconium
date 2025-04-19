#pragma once

#include "Entity.h"

namespace zirconium {
class Entity;

class ScriptableEntity {
public:
    virtual ~ScriptableEntity() = default;

    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate(float dt) {}

    template <typename T>
    T& GetComponent() {
        return m_Entity.GetComponent<T>();
    }

protected:
    Entity m_Entity;
    friend class Scene;
};
} // namespace zirconium
