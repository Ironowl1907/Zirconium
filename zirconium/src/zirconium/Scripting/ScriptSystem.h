#pragma once

#include "zirconium/Core/Timestep.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Scene.h"
#include <filesystem>

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

    bool LoadScript2Component(LuaScriptedComponent& scComponent, std::filesystem::path& scriptPath);

    ScriptingSystem(const ScriptingSystem&) = delete;
    ScriptingSystem& operator=(const ScriptingSystem&) = delete;

private:
    ScriptingSystem();
    ~ScriptingSystem();

private:
    static ScriptingSystem* m_Instance;
    Scene* m_Scene = nullptr;
};

} // namespace zirconium
