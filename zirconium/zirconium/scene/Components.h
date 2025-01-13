#pragma once
#include "SceneCamera.h"
#include "glm/glm.hpp"

#include "SceneCamera.h"

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

    CameraComponent() = default;
};

} // namespace zirconium
