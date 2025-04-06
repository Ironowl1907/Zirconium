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

struct IDComponent {
    UUID ID;

    IDComponent()
        : ID() {}
    IDComponent(uint64_t id)
        : ID(id) {}
};

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
};

struct TransformComponent {
    glm::vec3 Translation{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    TransformComponent() = default;
    TransformComponent(const glm::vec3& translation)
        : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1.0f, 0.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.y, {0.0f, 1.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.z, {0.0f, 0.0f, 1.0f});
        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("GetTranslation", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* t = registry.try_get<TransformComponent>(entity);
            if (!t)
                return sol::nil;

            sol::table tbl = lua.create_table();
            tbl["x"] = t->Translation.x;
            tbl["y"] = t->Translation.y;
            tbl["z"] = t->Translation.z;
            return tbl;
        });

        lua.set_function("SetTranslation", [&registry](entt::entity entity, float x, float y, float z) {
            auto* t = registry.try_get<TransformComponent>(entity);
            if (t) {
                t->Translation = {x, y, z};
            }
        });
    }
};

struct SpriteRendererComponent {
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

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("GetSpriteColor", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* sprite = registry.try_get<SpriteRendererComponent>(entity);
            if (!sprite)
                return sol::nil;

            sol::table color = lua.create_table();
            color["r"] = sprite->Color.r;
            color["g"] = sprite->Color.g;
            color["b"] = sprite->Color.b;
            color["a"] = sprite->Color.a;
            return color;
        });

        lua.set_function("SetSpriteColor", [&registry](entt::entity entity, float r, float g, float b, float a) {
            SpriteRendererComponent* sprite = registry.try_get<SpriteRendererComponent>(entity);
            if (sprite) {
                sprite->Color = {r, g, b, a};
            }
        });
    }
};

struct CameraComponent {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent()
        : Camera() {}

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("IsPrimaryCamera", [&registry](entt::entity entity) -> bool {
            auto* cam = registry.try_get<CameraComponent>(entity);
            return cam ? cam->Primary : false;
        });

        lua.set_function("SetPrimaryCamera", [&registry](entt::entity entity, bool primary) {
            auto* cam = registry.try_get<CameraComponent>(entity);
            if (cam)
                cam->Primary = primary;
        });
    }
};

struct RigidBodyComponent {
    enum class BodyType { Static = 0, Dynamic = 1, Kinematics = 2 };
    BodyType Type = BodyType::Dynamic;
    bool FixedRotation = false;
    b2BodyId RuntimeBody;

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("SetFixedRotation", [&registry](entt::entity entity, bool fixed) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb)
                rb->FixedRotation = fixed;
        });

        lua.set_function("GetBodyType", [&registry](entt::entity entity) -> int {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? static_cast<int>(rb->Type) : -1;
        });

        lua.set_function("SetBodyType", [&registry](entt::entity entity, int type) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb && type >= 0 && type <= 2)
                rb->Type = static_cast<RigidBodyComponent::BodyType>(type);
        });
    }
};

struct BoxColiderComponent {
    glm::vec2 Offset{0.0f};
    glm::vec2 Size{0.5f};
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("GetBoxCollider", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* c = registry.try_get<BoxColiderComponent>(entity);
            if (!c)
                return sol::nil;

            sol::table tbl = lua.create_table();
            tbl["offset_x"] = c->Offset.x;
            tbl["offset_y"] = c->Offset.y;
            tbl["size_x"] = c->Size.x;
            tbl["size_y"] = c->Size.y;
            tbl["density"] = c->Density;
            tbl["friction"] = c->Friction;
            tbl["restitution"] = c->Restitution;
            return tbl;
        });

        lua.set_function("SetBoxCollider",
                         [&registry](entt::entity entity, float offset_x, float offset_y, float size_x, float size_y,
                                     float density, float friction, float restitution) {
                             auto* c = registry.try_get<BoxColiderComponent>(entity);
                             if (c) {
                                 c->Offset = {offset_x, offset_y};
                                 c->Size = {size_x, size_y};
                                 c->Density = density;
                                 c->Friction = friction;
                                 c->Restitution = restitution;
                             }
                         });
    }
};

struct CircleRendererComponent {
    glm::vec4 Color{1.0f};
    float Thickness = 1.0f;
    float Fade = 0.0025f;

    operator glm::vec4&() {
        return Color;
    }
    operator const glm::vec4&() const {
        return Color;
    }

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("GetCircleColor", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (!circle)
                return sol::nil;

            sol::table color = lua.create_table();
            color["r"] = circle->Color.r;
            color["g"] = circle->Color.g;
            color["b"] = circle->Color.b;
            color["a"] = circle->Color.a;
            return color;
        });

        lua.set_function("SetCircleColor", [&registry](entt::entity entity, float r, float g, float b, float a) {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (circle)
                circle->Color = {r, g, b, a};
        });

        lua.set_function("GetCircleThickness", [&registry](entt::entity entity) -> sol::optional<float> {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (!circle)
                return sol::nullopt;
            return circle->Thickness;
        });

        lua.set_function("SetCircleThickness", [&registry](entt::entity entity, float thickness) {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (circle)
                circle->Thickness = thickness;
        });

        lua.set_function("GetCircleFade", [&registry](entt::entity entity) -> sol::optional<float> {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (!circle)
                return sol::nullopt;
            return circle->Fade;
        });

        lua.set_function("SetCircleFade", [&registry](entt::entity entity, float fade) {
            auto* circle = registry.try_get<CircleRendererComponent>(entity);
            if (circle)
                circle->Fade = fade;
        });
    }
};

struct CircleColiderComponent {
    glm::vec2 Offset{0.0f};
    float Radius = 0.5f;
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("GetCircleCollider", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* c = registry.try_get<CircleColiderComponent>(entity);
            if (!c)
                return sol::nil;

            sol::table tbl = lua.create_table();
            tbl["offset_x"] = c->Offset.x;
            tbl["offset_y"] = c->Offset.y;
            tbl["radius"] = c->Radius;
            tbl["density"] = c->Density;
            tbl["friction"] = c->Friction;
            tbl["restitution"] = c->Restitution;
            return tbl;
        });

        lua.set_function("SetCircleCollider",
                         [&registry](entt::entity entity, float offset_x, float offset_y, float radius, float density,
                                     float friction, float restitution) {
                             auto* c = registry.try_get<CircleColiderComponent>(entity);
                             if (c) {
                                 c->Offset = {offset_x, offset_y};
                                 c->Radius = radius;
                                 c->Density = density;
                                 c->Friction = friction;
                                 c->Restitution = restitution;
                             }
                         });
    }
};

struct NativeScriptComponent {
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
};

struct LuaScriptComponent {
    std::string ScriptPath;
    sol::table ScriptInstance;
};

} // namespace zirconium
