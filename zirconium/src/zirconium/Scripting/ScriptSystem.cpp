#include "ScriptSystem.h"
#include "zirconium/scene/Components.h"
#include "zrpch.h"
#include <filesystem>

namespace zirconium {

ScriptingSystem::~ScriptingSystem() {
    delete m_Instance;
}

void ScriptingSystem::UpdateScripts(TimeStep& deltatime) {
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptedComponent>();

    for (auto entity : view) {
        LuaScriptedComponent& scriptComponent = view.get<LuaScriptedComponent>(entity);

        scriptComponent.OnUpdate((float)deltatime);
    }
}
void ScriptingSystem::InitScripts() {
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptedComponent>();

    for (auto entity : view) {
        LuaScriptedComponent& scriptComponent = view.get<LuaScriptedComponent>(entity);

        scriptComponent.OnInit();
    }
}

bool ScriptingSystem::LoadScript2Component(LuaScriptedComponent& scComponent, std::filesystem::path& scriptPath) {
    ZR_ASSERT(std::filesystem::exists(scriptPath), "Path '{}' couldn't be found!");

    scComponent.LuaState.open_libraries(sol::lib::base);
    scComponent.LuaState.script_file(scriptPath); // Execute script file

    scComponent.OnUpdate = scComponent.LuaState["OnUpdate"];
    scComponent.OnInit = scComponent.LuaState["OnInit"];

    if (!scComponent.OnInit || !scComponent.OnUpdate())
        return false;
    return true;
}

} // namespace zirconium
