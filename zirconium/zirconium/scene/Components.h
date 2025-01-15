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
    operator const glm::mat4&() const {
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
    SceneCamrera Camera;
    bool Primary = true; // TODO: Think about moving to scene
    bool FixedAspectRatio = false;

    CameraComponent()
        : Camera() {}
};

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    std::function<void()> InstanciateFunction;
    std::function<void()> DestroyInstanceFunction;

    std::function<void(ScriptableEntity*)> OnCreateFunction;
    std::function<void(ScriptableEntity*)> OnDestroyFunction;
    std::function<void(ScriptableEntity*, TimeStep)> OnUpdateFunction;

    template <typename T>
    void Bind() {

        InstanciateFunction = [&]() {
            Instance = new T();
        };

        DestroyInstanceFunction = [&]() {
            delete static_cast<T*>(Instance);
            Instance = nullptr;
        };

        OnCreateFunction = [](ScriptableEntity* instance) {
            static_cast<T*>(instance)->OnCreate();
        };
        OnDestroyFunction = [](ScriptableEntity* instance) {
            static_cast<T*>(instance)->OnDestroy();
        };
        OnUpdateFunction = [](ScriptableEntity* instance, TimeStep delta) {
            static_cast<T*>(instance)->OnUpdate(delta);
        };
    }
};

} // namespace zirconium
