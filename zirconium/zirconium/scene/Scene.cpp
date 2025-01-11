#include "glm/glm.hpp"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/scene/Components.h"

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

entt::entity Scene::CreateEntity() {
    return m_Registry.create();
}
} // namespace zirconium
