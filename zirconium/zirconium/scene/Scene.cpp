#include "glm/glm.hpp"
#include "zrpch.h"

#include "Scene.h"

namespace zirconium {

Scence::Scence() {
    struct MeshComponent {
        int Mesh;

        MeshComponent() = default;

        MeshComponent(const MeshComponent&) = default;

        operator const int&() const {
            return Mesh;
        }
        operator int&() {
            return Mesh;
        }

        MeshComponent(const int mesh)
            : Mesh(mesh) {}
    };
    struct TransformComponent {
        glm::mat4 Transform;

        TransformComponent() = default;

        TransformComponent(const TransformComponent&) = default;

        operator const glm::mat4&() const {
            return Transform;
        }
        operator glm::mat4&() {
            return Transform;
        }

        TransformComponent(const glm::mat4 transform)
            : Transform(transform) {}
    };

    entt::entity entity = m_Registry.Create();
    m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

    if (m_Registry.has<TransformComponent>(entity))
        TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

    auto view = m_Registry.view<TransformComponent>();

    for (auto entity : view)
        TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

    auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
    for (auto entity : group)
        auto& [transform, mesh] = m_Registry.get<TransformComponent, MeshComponent>(entity);
}
Scence::~Scence() {}
} // namespace zirconium
