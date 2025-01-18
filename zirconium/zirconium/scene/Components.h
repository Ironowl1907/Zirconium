#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace zirconium {

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

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const glm::vec4& color)
        : Color(color) {}

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

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstanciateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template <typename T>
    void Bind() {

        InstanciateScript = []() {
            return static_cast<ScriptableEntity*>(new T());
        };

        DestroyScript = [](NativeScriptComponent* nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }
};

} // namespace zirconium
