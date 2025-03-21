#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SceneCamera.h"
#include "zirconium/Core/UUID.h"
#include "zirconium/Renderer/Texture.h"

#include "box2d/id.h"
#include "sol/sol.hpp"
#include <cstdint>
#include <filesystem>

namespace zirconium {

struct IDComponent {
    IDComponent()
        : ID() {}
    IDComponent(uint64_t id)
        : ID(id) {}
    UUID ID;
};

struct TagComponent {
    std::string Tag;

    TagComponent() = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
};
struct TransformComponent {
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;
    TransformComponent(const glm::vec4& translation)
        : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1.0f, 0.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.y, {0.0f, 1.0f, 0.0f}) *
                             glm::rotate(glm::mat4(1.0f), Rotation.z, {0.0f, 0.0f, 1.0f});

        return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct SpriteRendererComponent {
    glm::vec4 Color{1.0f};
    Ref<Texture2D> Texture;
    float TilingFactor = 1;

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

    operator glm::vec4&() {
        return Color;
    }
    operator const glm::vec4&() const {
        return Color;
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
};

struct CameraComponent {
    SceneCamera Camera;
    bool Primary = true; // TODO: Think about moving to scene
    bool FixedAspectRatio = false;

    CameraComponent()
        : Camera() {}
};

class SciptableEntity;

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

struct RigidBodyComponent {
    enum class BodyType { Static = 0, Dynamic = 1, Kinematics = 2 };
    BodyType Type = BodyType::Dynamic;

    bool FixedRotation = false;

    b2BodyId RuntimeBody;
};

struct BoxColiderComponent {

    glm::vec2 Offset = {0.0f, 0.0f};
    glm::vec2 Size = {0.5f, 0.5f};

    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
};

struct CircleColiderComponent {

    glm::vec2 Offset = {0.0f, 0.0f};
    float Radius = 0.5f;

    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
};

struct LuaScriptedComponent {
    sol::state LuaState;

    std::filesystem::path ScriptPath = "";

    sol::function OnUpdate;
    sol::function OnInit;
};

} // namespace zirconium
