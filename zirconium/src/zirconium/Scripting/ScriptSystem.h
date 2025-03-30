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

    bool LoadScript2Entity(Entity& entity, std::filesystem::path scriptPath);
    void RegisterComponentsToLua(sol::state& lua);
    ScriptingSystem(const ScriptingSystem&) = delete;
    ScriptingSystem& operator=(const ScriptingSystem&) = delete;

    void LoadTypes(sol::state& state);

private:
    ScriptingSystem();
    ~ScriptingSystem();

    void RegisterVectors(sol::state& lua);
    // void RegisterEntity(sol::state& lua);

private:
    static ScriptingSystem* m_Instance;
    Scene* m_Scene = nullptr;

    std::unordered_map<UUID, sol::state> m_LuaStates;
};

} // namespace zirconium
