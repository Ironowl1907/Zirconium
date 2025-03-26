#include "ScriptSystem.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Entity.h"
#include "zrpch.h"
#include <filesystem>

namespace zirconium {

ScriptingSystem* ScriptingSystem::m_Instance = nullptr;

ScriptingSystem::ScriptingSystem() {}

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

bool ScriptingSystem::LoadScript2Entity(Entity& entity, std::filesystem::path scriptPath) {
    ZR_ASSERT(std::filesystem::exists(scriptPath), "Path '{}' couldn't be found!");
    ZR_ASSERT(entity.HasComponent<LuaScriptedComponent>(), "Tring to load script without LuaScriptedComponent!")

    sol::state& luaState = m_LuaStates[entity.GetID()]; // Get or create state
    LuaScriptedComponent& scComponent = entity.GetComponent<LuaScriptedComponent>();

    luaState.open_libraries(sol::lib::base);

    try {
        luaState.safe_script_file(scriptPath); // Execute script file
    } catch (const sol::error& e) {
        ZR_ERROR("Error loading lua script! \n {}", e.what());
        return false;
    }

    scComponent.OnUpdate = luaState["OnUpdate"];
    scComponent.OnInit = luaState["OnInit"];

    ZR_WARN("OnUpdate {0}, OnInit {1}", (bool)scComponent.OnUpdate, (bool)scComponent.OnInit);

    scComponent.ScriptPath = scriptPath;
    return (bool)scComponent.OnUpdate && (bool)scComponent.OnInit;
}

} // namespace zirconium
