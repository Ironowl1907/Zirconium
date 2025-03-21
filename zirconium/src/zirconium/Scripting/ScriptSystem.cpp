#include "ScriptSystem.h"
#include "zrpch.h"

namespace zirconium {

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

} // namespace zirconium
