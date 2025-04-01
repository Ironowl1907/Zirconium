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

void ScriptingSystem::UpdateScripts(TimeStep& deltatime) {
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();

    for (auto entity : view) {
        LuaScriptComponent& scriptComponent = view.get<LuaScriptComponent>(entity);

        scriptComponent.OnUpdate((float)deltatime);
    }
}
void ScriptingSystem::InitScripts() {
    auto view = m_Scene->GetAllEntitiesWith<LuaScriptComponent>();

    for (auto entity : view) {
        LuaScriptComponent& scriptComponent = view.get<LuaScriptComponent>(entity);

        scriptComponent.OnInit();
    }
}

bool ScriptingSystem::LoadScript2Entity(Entity& entity, std::filesystem::path scriptPath) {
    ZR_ASSERT(std::filesystem::exists(scriptPath), "Path '{}' couldn't be found!");
    ZR_ASSERT(entity.HasComponent<LuaScriptComponent>(), "Trying to load script without LuaScriptComponent!");
    ZR_ASSERT((bool)entity, "Entity not vaild");

    sol::state& luaState = m_LuaStates[entity.GetID()];
    LuaScriptComponent& scComponent = entity.GetComponent<LuaScriptComponent>();

    scComponent.LuaState = &luaState;

    luaState.open_libraries(sol::lib::base);

    // Register components
    RegisterComponentsToLua(luaState);
    RegisterVectors(luaState);

    uint64_t entityID = static_cast<uint64_t>(entity);
    luaState.new_usertype<Entity>(     //
        "Entity",                      //
        sol::constructors<Entity()>(), //
        "GetID", [entityID](Entity& e) -> uint64_t {
            // ZR_ASSERT((bool)e, "Entity not vaild");
            return entityID;
        });

    ZR_CORE_WARN("Entity: {}", static_cast<uint64_t>(entity));
    luaState["GetEntity"] = [&entity]() -> Entity& {
        return entity;
    };

    try {
        luaState.safe_script_file(scriptPath); // Execute script file
    } catch (const sol::error& e) {
        ZR_ERROR("Error loading lua script! \n{}", e.what());
        return false;
    }

    scComponent.OnUpdate = luaState["OnUpdate"];
    scComponent.OnInit = luaState["OnInit"];

    ZR_WARN("Loaded functions\n\t{0}\n\t{1}", ((bool)scComponent.OnUpdate) ? "OnUpdate()" : "",
            ((bool)scComponent.OnInit) ? "OnInit()" : "");

    scComponent.ScriptPath = scriptPath;
    return (bool)scComponent.OnUpdate && (bool)scComponent.OnInit;
}

void ScriptingSystem::RegisterComponentsToLua(sol::state& lua) {

    // Transform Component
    lua.new_usertype<zirconium::TransformComponent>(                //
        "TransformComponent",                                       //
        "Translation", &zirconium::TransformComponent::Translation, //
        "Rotation", &zirconium::TransformComponent::Rotation,       //
        "Scale", &zirconium::TransformComponent::Scale,             //
        "GetTransform", &zirconium::TransformComponent::GetTransform);

    // Tag Component
    lua.new_usertype<zirconium::TagComponent>("TagComponent", "Tag", &zirconium::TagComponent::Tag);

    // SpriteRenderer Component
    lua.new_usertype<zirconium::SpriteRendererComponent>("SpriteRendererComponent", "Color",
                                                         &zirconium::SpriteRendererComponent::Color);

    // CircleRenderer Compoent
    lua.new_usertype<zirconium::CircleRendererComponent>(
        "CircleRendererComponent", "Color", &zirconium::CircleRendererComponent::Color, "Thickness",
        &zirconium::CircleRendererComponent::Thickness, "Fade", &zirconium::CircleRendererComponent::Fade);

    // Camera Compoent
    lua.new_usertype<zirconium::CameraComponent>("CameraComponent", "Camera", &zirconium::CameraComponent::Camera,
                                                 "Primary", &zirconium::CameraComponent::Primary, "FixedAspectRatio",
                                                 &zirconium::CameraComponent::FixedAspectRatio);
}

void ScriptingSystem::LoadTypes(sol::state& state) {
    RegisterVectors(state);
}

void ScriptingSystem::RegisterVectors(sol::state& lua) {
    lua.new_usertype<glm::vec3>(
        "vec3", //
        sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(), "x", &glm::vec3::x, "y", &glm::vec3::y, "z",
        &glm::vec3::z,

        // Operator overloads
        sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(&glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(&glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, float)>(&glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, float)>(&glm::operator/));

    lua.new_usertype<glm::vec2>(
        "vec2", //
        sol::constructors<glm::vec2(), glm::vec2(float, float)>(), "x", &glm::vec2::x, "y", &glm::vec2::y,

        // Operator overloads
        sol::meta_function::addition, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(&glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(&glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, float)>(&glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, float)>(&glm::operator/));
}

} // namespace zirconium
