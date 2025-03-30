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
    virtual void ExposeAtributesLua(sol::state& lua, entt::registry& registry) = 0;
};

struct IDComponent {
    UUID ID;

    IDComponent()
        : ID() {}
    IDComponent(uint64_t id)
        : ID(id) {}
};

struct TagComponent : public LuaScripted {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<TagComponent>("TagComponent",           //
                                       "Tag", &TagComponent::Tag //
        );
        lua["GetTagComponent"] = [&registry](uint64_t entity) -> TagComponent& {
            return registry.get<TagComponent>(static_cast<entt::entity>(entity));
        };
    }
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

    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<TransformComponent>("TransformComponent", "Translation", &TransformComponent::Translation, //
                                             "Rotation", &TransformComponent::Rotation,                             //
                                             "Scale", &TransformComponent::Scale,                                   //
                                             "GetTransform", &TransformComponent::GetTransform                      //
        );
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
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<SpriteRendererComponent>("SpriteRendererComponent",                             //
                                                  "Color", &SpriteRendererComponent::Color,              //
                                                  "TilingFactor", &SpriteRendererComponent::TilingFactor //
        );
    }
};

struct CameraComponent : public LuaScripted {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent()
        : Camera() {}
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<CameraComponent>("CameraComponent", "Primary", &CameraComponent::Primary, //
                                          "FixedAspectRatio", &CameraComponent::FixedAspectRatio   //
        );
    }
};

struct RigidBodyComponent : public LuaScripted {
    enum class BodyType { Static = 0, Dynamic = 1, Kinematics = 2 };
    BodyType Type = BodyType::Dynamic;
    bool FixedRotation = false;
    b2BodyId RuntimeBody;
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<RigidBodyComponent>("RigidBodyComponent", "Type", &RigidBodyComponent::Type, //
                                             "FixedRotation", &RigidBodyComponent::FixedRotation      //
        );
    }
};

struct BoxColiderComponent : public LuaScripted {
    glm::vec2 Offset{0.0f};
    glm::vec2 Size{0.5f};
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<BoxColiderComponent>("BoxColiderComponent",                           //
                                              "Offset", &BoxColiderComponent::Offset,          //
                                              "Size", &BoxColiderComponent::Size,              //
                                              "Density", &BoxColiderComponent::Density,        //
                                              "Friction", &BoxColiderComponent::Friction,      //
                                              "Restitution", &BoxColiderComponent::Restitution //
        );
    }
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
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<CircleRendererComponent>("CircleRendererComponent",                        //
                                                  "Color", &CircleRendererComponent::Color,         //
                                                  "Thickness", &CircleRendererComponent::Thickness, //
                                                  "Fade", &CircleRendererComponent::Fade            //
        );
    }
};

struct CircleColiderComponent : public LuaScripted {
    glm::vec2 Offset{0.0f};
    float Radius = 0.5f;
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    void ExposeAtributesLua(sol::state& lua, entt::registry& registry) override {
        lua.new_usertype<CircleColiderComponent>(
            "CircleColiderComponent", "Offset", &CircleColiderComponent::Offset, "Radius",
            &CircleColiderComponent::Radius, "Density", &CircleColiderComponent::Density, "Friction",
            &CircleColiderComponent::Friction, "Restitution", &CircleColiderComponent::Restitution);

        lua["GetCircleColiderComponent"] = [&registry](uint64_t entity) -> CircleColiderComponent& {
            return registry.get<CircleColiderComponent>(static_cast<entt::entity>(entity));
        };
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
    Ref<sol::state> LuaState;

    std::filesystem::path ScriptPath = "";

    sol::function OnUpdate;
    sol::function OnInit;
};

} // namespace zirconium
