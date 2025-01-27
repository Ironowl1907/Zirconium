#include "Components.h"
#include "Entity.h"
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

void Scene::OnUpdateEditor(TimeStep delta, EditorCamera& camera) {
    Renderer2D::BeginScene(camera);

    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        Renderer2D::DrawTransformedQuad(transform.GetTransform(), (glm::vec4)sprite);
    }
    Renderer2D::EndScene();
}

void Scene::OnUpdateRuntime(TimeStep delta) {
    // Update scripts
    {
        m_Registry.view<NativeScriptComponent>().each([this, &delta](auto entity, auto& nsc) {
            // Move to the game runtime `Scene::OnScenePlay`
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstanciateScript();
                nsc.Instance->m_Entity = Entity{entity, this};
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(delta);
        });
    }

    // Render Sprites
    Camera* mainCamera = nullptr;
    glm::mat4 mainTransform;
    {
        auto group = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : group) {
            const auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary) {
                mainCamera = &camera.Camera;
                mainTransform = transform.GetTransform();
                break;
            }
        }
    }

    if (mainCamera) {
        Renderer2D::BeginScene(*mainCamera, mainTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawTransformedQuad(transform.GetTransform(), (glm::vec4)sprite);
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

void Scene::OnViewportResize(const uint32_t& width, const uint32_t& height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    // Resize our non-fixed aspect ratio camera
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.FixedAspectRatio) {
            cameraComponent.Camera.SetViewportSize(width, height);
        }
    }
}

void Scene::DeleteEntity(Entity entity) {
    m_Registry.destroy((entt::entity)entity);
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
    static_assert(false);
}

template <>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
    component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}

template <>
void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

template <>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

} // namespace zirconium
