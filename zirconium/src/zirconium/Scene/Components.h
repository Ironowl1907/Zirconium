#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SceneCamera.h"
#include "zirconium/Core/UUID.h"
#include "zirconium/Renderer/Texture.h"

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "entt.hpp"
#include "sol/sol.hpp"
#define SOL_ALL_SAFIEST_ON 1
#include <cstdint>
#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }

    static void Expose2Lua(sol::state& lua, entt::registry& registry) {
        lua.set_function("TransformComponent_GetTranslation", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* t = registry.try_get<TransformComponent>(entity);
            if (!t)
                return sol::nil;

            sol::table tbl = lua.create_table();
            tbl["x"] = t->Translation.x;
            tbl["y"] = t->Translation.y;
            tbl["z"] = t->Translation.z;
            return tbl;
        });

        lua.set_function("TransformComponent_SetTranslation",
                         [&registry](entt::entity entity, float x, float y, float z) {
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

        lua.set_function("SetSpriteTexture", [&registry](entt::entity entity, std::string path) {
            SpriteRendererComponent* sprite = registry.try_get<SpriteRendererComponent>(entity);
            if (sprite) {
                sprite->Texture = Texture2DLibrary::Get()->Add(path);
            }
        });

        lua.set_function("GetSpriteTexture", [&registry](entt::entity entity) -> std::string {
            SpriteRendererComponent* sprite = registry.try_get<SpriteRendererComponent>(entity);
            if (sprite) {
                return sprite->Texture->GetPath();
            }
            return "";
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
        lua.set_function("CameraComponent_IsPrimaryCamera", [&registry](entt::entity entity) -> bool {
            auto* cam = registry.try_get<CameraComponent>(entity);
            return cam ? cam->Primary : false;
        });

        lua.set_function("CameraComponent_SetPrimaryCamera", [&registry](entt::entity entity, bool primary) {
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
        lua.set_function("RigidBodyComponent_SetFixedRotation", [&registry](entt::entity entity, bool fixed) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb)
                rb->FixedRotation = fixed;
        });

        lua.set_function("RigidBodyComponent_GetBodyType", [&registry](entt::entity entity) -> int {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? static_cast<int>(rb->Type) : -1;
        });

        lua.set_function("RigidBodyComponent_GetRuntimeBody", [&registry](entt::entity entity) -> b2BodyId {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? rb->RuntimeBody : b2BodyId{};
        });

        lua.set_function("RigidBodyComponent_SetBodyType", [&registry](entt::entity entity, int type) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb && type >= 0 && type <= 2)
                rb->Type = static_cast<RigidBodyComponent::BodyType>(type);
        });

        lua.set_function("RigidBodyComponent_SetTransformation",
                         [&registry](entt::entity entity, float posx, float posy, float rotSen, float rotCos) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_SetTransform(rb->RuntimeBody, {posx, posy}, {rotSen, rotCos});
                             }
                         });

        lua.set_function("RigidBodyComponent_GetLinearVelocity", [&registry, &lua](entt::entity entity) -> sol::table {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (!rb)
                return sol::nil;

            auto velocity = b2Body_GetLinearVelocity(rb->RuntimeBody);
            sol::table result = lua.create_table();
            result["x"] = velocity.x;
            result["y"] = velocity.y;
            return result;
        });

        lua.set_function("RigidBodyComponent_GetAngularVelocity", [&registry](entt::entity entity) -> float {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_GetAngularVelocity(rb->RuntimeBody) : 0.0f;
        });

        lua.set_function("RigidBodyComponent_SetLinearVelocity",
                         [&registry](entt::entity entity, float velx, float vely) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_SetLinearVelocity(rb->RuntimeBody, {velx, vely});
                             }
                         });

        lua.set_function("RigidBodyComponent_SetAngularVelocity",
                         [&registry](entt::entity entity, float angularVelocity) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_SetAngularVelocity(rb->RuntimeBody, angularVelocity);
                             }
                         });

        lua.set_function(
            "RigidBodyComponent_SetTargetTransform",
            [&registry](entt::entity entity, float posx, float posy, float rotSen, float rotCos, float timeStep) {
                auto* rb = registry.try_get<RigidBodyComponent>(entity);
                if (rb) {
                    b2Transform target = {{posx, posy}, {rotSen, rotCos}};
                    b2Body_SetTargetTransform(rb->RuntimeBody, target, timeStep);
                }
            });

        lua.set_function("RigidBodyComponent_GetLocalPointVelocity",
                         [&registry, &lua](entt::entity entity, float pointx, float pointy) -> sol::table {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (!rb)
                                 return sol::nil;

                             auto velocity = b2Body_GetLocalPointVelocity(rb->RuntimeBody, {pointx, pointy});
                             sol::table result = lua.create_table();
                             result["x"] = velocity.x;
                             result["y"] = velocity.y;
                             return result;
                         });

        lua.set_function("RigidBodyComponent_GetWorldPointVelocity",
                         [&registry, &lua](entt::entity entity, float pointx, float pointy) -> sol::table {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (!rb)
                                 return sol::nil;

                             auto velocity = b2Body_GetWorldPointVelocity(rb->RuntimeBody, {pointx, pointy});
                             sol::table result = lua.create_table();
                             result["x"] = velocity.x;
                             result["y"] = velocity.y;
                             return result;
                         });

        lua.set_function("RigidBodyComponent_ApplyForce", [&registry](entt::entity entity, float forcex, float forcey,
                                                                      float pointx, float pointy, bool wake) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb) {
                b2Body_ApplyForce(rb->RuntimeBody, {forcex, forcey}, {pointx, pointy}, wake);
            }
        });

        lua.set_function("RigidBodyComponent_ApplyForceToCenter",
                         [&registry](entt::entity entity, float forcex, float forcey, bool wake) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_ApplyForceToCenter(rb->RuntimeBody, {forcex, forcey}, wake);
                             }
                         });

        lua.set_function("RigidBodyComponent_ApplyTorque", [&registry](entt::entity entity, float torque, bool wake) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb) {
                b2Body_ApplyTorque(rb->RuntimeBody, torque, wake);
            }
        });

        lua.set_function(
            "RigidBodyComponent_ApplyLinearImpulse",
            [&registry](entt::entity entity, float impulsex, float impulsey, float pointx, float pointy, bool wake) {
                auto* rb = registry.try_get<RigidBodyComponent>(entity);
                if (rb) {
                    b2Body_ApplyLinearImpulse(rb->RuntimeBody, {impulsex, impulsey}, {pointx, pointy}, wake);
                }
            });

        lua.set_function("RigidBodyComponent_ApplyLinearImpulseToCenter",
                         [&registry](entt::entity entity, float impulsex, float impulsey, bool wake) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_ApplyLinearImpulseToCenter(rb->RuntimeBody, {impulsex, impulsey}, wake);
                             }
                         });

        lua.set_function("RigidBodyComponent_ApplyAngularImpulse",
                         [&registry](entt::entity entity, float impulse, bool wake) {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (rb) {
                                 b2Body_ApplyAngularImpulse(rb->RuntimeBody, impulse, wake);
                             }
                         });

        lua.set_function("RigidBodyComponent_GetMass", [&registry](entt::entity entity) -> float {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_GetMass(rb->RuntimeBody) : 0.0f;
        });

        lua.set_function("RigidBodyComponent_GetRotationalInertia", [&registry](entt::entity entity) -> float {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_GetRotationalInertia(rb->RuntimeBody) : 0.0f;
        });

        lua.set_function("RigidBodyComponent_GetLocalCenterOfMass",
                         [&registry, &lua](entt::entity entity) -> sol::table {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (!rb)
                                 return sol::nil;

                             auto center = b2Body_GetLocalCenterOfMass(rb->RuntimeBody);
                             sol::table result = lua.create_table();
                             result["x"] = center.x;
                             result["y"] = center.y;
                             return result;
                         });

        lua.set_function("RigidBodyComponent_GetWorldCenterOfMass",
                         [&registry, &lua](entt::entity entity) -> sol::table {
                             auto* rb = registry.try_get<RigidBodyComponent>(entity);
                             if (!rb)
                                 return sol::nil;

                             auto center = b2Body_GetWorldCenterOfMass(rb->RuntimeBody);
                             sol::table result = lua.create_table();
                             result["x"] = center.x;
                             result["y"] = center.y;
                             return result;
                         });

        lua.set_function("RigidBodyComponent_SetAwake", [&registry](entt::entity entity, bool awake) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb) {
                b2Body_SetAwake(rb->RuntimeBody, awake);
            }
        });

        lua.set_function("RigidBodyComponent_IsAwake", [&registry](entt::entity entity) -> bool {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_IsAwake(rb->RuntimeBody) : false;
        });

        lua.set_function("RigidBodyComponent_SetBullet", [&registry](entt::entity entity, bool flag) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb) {
                b2Body_SetBullet(rb->RuntimeBody, flag);
            }
        });

        lua.set_function("RigidBodyComponent_IsBullet", [&registry](entt::entity entity) -> bool {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_IsBullet(rb->RuntimeBody) : false;
        });

        lua.set_function("RigidBodyComponent_SetFixedRotation", [&registry](entt::entity entity, bool flag) {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            if (rb) {
                b2Body_SetFixedRotation(rb->RuntimeBody, flag);
            }
        });

        lua.set_function("RigidBodyComponent_IsFixedRotation", [&registry](entt::entity entity) -> bool {
            auto* rb = registry.try_get<RigidBodyComponent>(entity);
            return rb ? b2Body_IsFixedRotation(rb->RuntimeBody) : false;
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
        lua.set_function("BoxColliderComponent_Get", [&registry, &lua](entt::entity entity) -> sol::table {
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

        lua.set_function("BoxColliderComponent_Set",
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
        lua.set_function("CircleRendererComponent_GetCircleColor",
                         [&registry, &lua](entt::entity entity) -> sol::table {
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

        lua.set_function("CircleRendererComponent_SetCircleColor",
                         [&registry](entt::entity entity, float r, float g, float b, float a) {
                             auto* circle = registry.try_get<CircleRendererComponent>(entity);
                             if (circle)
                                 circle->Color = {r, g, b, a};
                         });

        lua.set_function("CircleRendererComponent_GetCircleThickness",
                         [&registry](entt::entity entity) -> sol::optional<float> {
                             auto* circle = registry.try_get<CircleRendererComponent>(entity);
                             if (!circle)
                                 return sol::nullopt;
                             return circle->Thickness;
                         });

        lua.set_function("CircleRendererComponent_SetCircleThickness",
                         [&registry](entt::entity entity, float thickness) {
                             auto* circle = registry.try_get<CircleRendererComponent>(entity);
                             if (circle)
                                 circle->Thickness = thickness;
                         });

        lua.set_function("CircleRendererComponent_GetCircleFade",
                         [&registry](entt::entity entity) -> sol::optional<float> {
                             auto* circle = registry.try_get<CircleRendererComponent>(entity);
                             if (!circle)
                                 return sol::nullopt;
                             return circle->Fade;
                         });

        lua.set_function("CircleRendererComponent_SetCircleFade", [&registry](entt::entity entity, float fade) {
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
        lua.set_function("CircleColiderComponent_GetCircleCollider",
                         [&registry, &lua](entt::entity entity) -> sol::table {
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

        lua.set_function("CircleColiderComponent_SetCircleCollider",
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
