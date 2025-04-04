#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SceneCamera.h"
#include "zirconium/Core/UUID.h"
#include "zirconium/Renderer/Texture.h"

#include "box2d/id.h"
#include "entt.hpp"
#include "sol/sol.hpp"
#define SOL_ALL_SAFIEST_ON 1
#include <cstdint>
#include <filesystem>

namespace zirconium {

struct LuaScripted {
    virtual ~LuaScripted() {}
    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) = 0;
};

struct IDComponent : LuaScripted {
    UUID ID;

    IDComponent()
        : ID() {}
    IDComponent(uint64_t id)
        : ID(id) {}
    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct TagComponent : public LuaScripted {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct TransformComponent : public LuaScripted {
    glm::vec3 Translation{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    TransformComponent() = default;
    TransformComponent(const glm::vec4& translation)
        : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1.0f, 0.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.y, {0.0f, 1.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.z, {0.0f, 0.0f, 1.0f});
        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {
        lua["getTransform"] = [](entt::entity entity) -> sol::table {
            auto* transform = registry.try_get<TransformComponent>(entity);
            if (!transform)
                return sol::nil;

            return sol::table(lua, {{"x", transform->x}, {"y", transform->y}});
        };

        lua["setTransform"] = [](entt::entity entity, float x, float y) {
            auto* transform = registry.try_get<TransformComponent>(entity);
            if (transform) {
                transform->x = x;
                transform->y = y;
            }
        };
    }
};

struct SpriteRendererComponent : public LuaScripted {
    glm::vec4 Color{1.0f};
    Ref<Texture2D> Texture;
    float TilingFactor = 1.0f;

    SpriteRendererComponent() {
        Texture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        Texture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));
    }
    SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {
        Texture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        Texture->SetData(&whiteTextureData, 1 * sizeof(whiteTextureData));
    }

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct CameraComponent : public LuaScripted {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent()
        : Camera() {}

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct RigidBodyComponent : public LuaScripted {
    enum class BodyType { Static = 0, Dynamic = 1, Kinematics = 2 };
    BodyType Type = BodyType::Dynamic;
    bool FixedRotation = false;
    b2BodyId RuntimeBody;

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct BoxColiderComponent : public LuaScripted {
    glm::vec2 Offset{0.0f};
    glm::vec2 Size{0.5f};
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct CircleRendererComponent : LuaScripted {
    glm::vec4 Color{1.0f};
    float Thickness = 1.0f;
    float Fade = 0.0025f;

    operator glm::vec4&() {
        return Color;
    }
    operator const glm::vec4&() const {
        return Color;
    }

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct CircleColiderComponent : public LuaScripted {
    glm::vec2 Offset{0.0f};
    float Radius = 0.5f;
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct NativeScriptComponent : public LuaScripted {
    class ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template <typename T>
    void Bind() {
        InstantiateScript = []() {
            return static_cast<ScriptableEntity*>(new T());
        };
        DestroyScript = [](NativeScriptComponent* nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }

    virtual void ExposeAPI2Lua(sol::state& lua, Entity& entity) {}
};

struct LuaScriptComponent {
    std::string ScriptPath;
    sol::table ScriptInstance;
};

} // namespace zirconium
