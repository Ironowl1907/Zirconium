#include "Components.h"
#include "glm/glm.hpp"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Entity.h"

#include "Scene.h"

namespace zirconium {

Scene::Scene() {
    m_Registry = entt::registry();
};

Scene::~Scene() {}

void Scene::OnUpdate(TimeStep delta) {
    // Render Sprites
    Camera* mainCamera = nullptr;
    glm::mat4* mainTransform = nullptr;
    {
        auto group = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : group) {
            const auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary) {
                mainCamera = &camera.Camera;
                mainTransform = &transform.Transform;
                break;
            }
        }
    }

    if (mainCamera) {
        Renderer2D::BeginScene(*mainCamera, *mainTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawTransformedQuad((glm::mat4)transform, (glm::vec4)sprite);
        }
        Renderer2D::EndScene();
    }
}

Entity Scene::CreateEntity(const std::string& name) {
    entt::entity ent = m_Registry.create();
    Entity entity = Entity(ent, this);
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = (name.empty()) ? "Entity" : name;

    return entity;
}
} // namespace zirconium
