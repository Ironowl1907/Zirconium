#pragma once

#include "zirconium/Core/Timestep.h"
#include "zirconium/scene/Scene.h"
#include "zirconium/scene/Component.h"

namespace zirconium {

class ScriptingSystem {
public:
    static ScriptingSystem* Get() {
        if (m_Instance == nullptr)
            m_Instance = new ScriptingSystem();
        return m_Instance;
    }

    void UpdateScripts(TimeStep& deltatime);
    void InitScripts();

    ScriptingSystem(const ScriptingSystem&) = delete;
    ScriptingSystem& operator=(const ScriptingSystem&) = delete;

private:
private:
    static ScriptingSystem* m_Instance = nullptr;
    Scene* m_Scene = nullptr;
};

} // namespace zirconium
