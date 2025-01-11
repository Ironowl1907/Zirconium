#include "Components.h"
#include "glm/glm.hpp"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Entity.h"

#include "Scene.h"

namespace zirconium {

Scene::Scene() {};

Scene::~Scene() {}

void Scene::OnUpdate(TimeStep delta) {
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

        Renderer2D::DrawTransformedQuad((glm::mat4)transform, (glm::vec4)sprite);
    }
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity = {m_Registry.create(), this};
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = (name.empty()) ? "Entiry" : name;

    return entity;
}
} // namespace zirconium
