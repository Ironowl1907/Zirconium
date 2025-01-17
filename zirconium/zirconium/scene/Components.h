#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"

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
    glm::mat4 Transform{1.0f};

    TransformComponent() = default;
    TransformComponent(const glm::mat4& transform)
        : Transform(transform) {}

    operator glm::mat4&() {
        return Transform;
    }
    operator const glm::mat4&() const {;
        return Transform;
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
