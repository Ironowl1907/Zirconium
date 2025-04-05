#pragma once

#include "zirconium/Core/Timestep.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Scene.h"
#include <filesystem>
#include <functional>

namespace zirconium {

class Scene;

class ScriptingSystem {
public:
    static ScriptingSystem* Get() {
        if (m_Instance == nullptr)
            m_Instance = new ScriptingSystem();
        return m_Instance;
    }

    void Init(Scene* scene);
    void UpdateScripts(TimeStep& deltatime);
    void InitScripts();

    bool LoadScript2Entity(Entity& entity, std::filesystem::path scriptPath);
    ScriptingSystem(const ScriptingSystem&) = delete;
    ScriptingSystem& operator=(const ScriptingSystem&) = delete;

    using LuaExposerFn = std::function<void(sol::state&, entt::registry&)>;

    static inline std::vector<LuaExposerFn>& GetLuaComponentRegistry() {
        static std::vector<LuaExposerFn> registry;
        return registry;
    }

    void RegisterAllComponentsToLua(sol::state& lua, entt::registry& registry);

private:
    ScriptingSystem();
    ~ScriptingSystem();

private:
    static ScriptingSystem* m_Instance;
    Scene* m_Scene = nullptr;

    // std::unordered_map<UUID, sol::state> m_LuaStates;
    sol::state m_LuaState;

    friend class Scene;
};

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)

#define REGISTER_COMPONENT_TO_LUA(Func)                                \
    static bool UNIQUE_NAME(_registered_) = [] {                       \
        ScriptingSystem::GetLuaComponentRegistry().emplace_back(Func); \
        return true;                                                   \
    }()

REGISTER_COMPONENT_TO_LUA(TransformComponent::Expose2Lua);
REGISTER_COMPONENT_TO_LUA(SpriteRendererComponent::Expose2Lua);

} // namespace zirconium
