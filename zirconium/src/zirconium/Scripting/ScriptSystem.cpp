#include "ScriptSystem.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Entity.h"
#include "zrpch.h"
#include <cstdint>
#include <filesystem>
#include <memory>

namespace zirconium {

ScriptingSystem* ScriptingSystem::m_Instance = nullptr;

ScriptingSystem::ScriptingSystem() {}

ScriptingSystem::~ScriptingSystem() {
    delete m_Instance;
}

void ScriptingSystem::Init(Scene* scene) {
    m_Scene = scene;
    m_LuaState.open_libraries(sol::lib::base);
}
void ScriptingSystem::UpdateScripts(TimeStep& deltatime) {
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();
    for (auto e : view) {
        Entity entity(e, m_Scene);
        auto& script = entity.GetComponent<LuaScriptComponent>();
        if (script.ScriptInstance["onUpdate"].valid()) {
            script.ScriptInstance["onUpdate"](e, (float)deltatime);
        }
    }
}
void ScriptingSystem::InitScripts() {
    ZR_ASSERT(m_Scene, "Scene is NULL!");
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();

    // Load ScriptInstance table
    sol::table scriptInstance = m_LuaState.script_file(scriptPath);
    luaScript.ScriptInstance = scriptInstance;

    for (auto e : view) {
        Entity entity(e, m_Scene);

        // Run onCreate function for entities
        sol::table& scriptInstance = entity.GetComponent<LuaScriptComponent>().ScriptInstance;
        if (scriptInstance["onCreate"].valid()) {
            scriptInstance["onCreate"](e, scriptInstance);
        }
    }
}

bool ScriptingSystem::LoadScript2Entity(Entity& entity, std::filesystem::path scriptPath) {
    auto& luaScript = entity.GetComponent<LuaScriptComponent>();

    ZR_CORE_TRACE("Loaded script");
    return true; // TODO: Fix returning
}

} // namespace zirconium
