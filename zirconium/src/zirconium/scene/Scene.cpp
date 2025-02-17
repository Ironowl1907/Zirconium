#include "Components.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/scene/Components.h"
#include "zirconium/scene/Entity.h"
#include <cstdint>

#include "Scene.h"
#include "box2d/box2d.h"

#include "box2d/id.h"

namespace zirconium {

static b2BodyType ZirconiumRigidBody2DToB2DRigidBodyType(RigidBodyComponent::BodyType bodyType) {
    switch (bodyType) {
    case RigidBodyComponent::BodyType::Static:
        return b2_staticBody;
    case RigidBodyComponent::BodyType::Dynamic:
        return b2_dynamicBody;
    case RigidBodyComponent::BodyType::Kinematics:
        return b2_kinematicBody;
    }
    ZR_ASSERT(false, "Invalid body type!");
    return b2_kinematicBody;
}

Scene::Scene() {
    m_Registry = entt::registry();
};

Scene::~Scene() {}

void Scene::OnUpdateEditor(TimeStep delta, EditorCamera& camera) {
    Renderer2D::BeginScene(camera);

    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto entity : group) {
        const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        Renderer2D::DrawSprite(transform.GetTransform(), sprite, (uint64_t)entity);
    }
    Renderer2D::EndScene();
}

void Scene::OnRuntimeStart() {
    b2WorldDef worldDefinition = b2DefaultWorldDef();
    worldDefinition.gravity = (b2Vec2){0.0f, -10.0f};

    m_WorldID = new b2WorldId;
    *m_WorldID = b2CreateWorld(&worldDefinition);

    auto view = m_Registry.view<RigidBodyComponent>();
    for (auto e : view) {
        Entity entity = {e, this};
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<RigidBodyComponent>();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = ZirconiumRigidBody2DToB2DRigidBodyType(rb2d.Type);
        bodyDef.position = {transform.Translation.x, transform.Translation.y};
        bodyDef.rotation = b2MakeRot(glm::radians(transform.Rotation.z));

        b2BodyId bodyId = b2CreateBody(*m_WorldID, &bodyDef);
        b2Body_SetFixedRotation(bodyId, rb2d.FixedRotation);

        if (entity.HasComponent<BoxColiderComponent>()) {
            auto& b2cc = entity.GetComponent<BoxColiderComponent>();

            b2Polygon colider = b2MakeBox(b2cc.Size.x * transform.Scale.x, b2cc.Size.y * transform.Scale.y);

            b2ShapeDef shapeDef = b2DefaultShapeDef();
            b2CreatePolygonShape(bodyId, &shapeDef, &colider);

            shapeDef.density = b2cc.Density;
            shapeDef.restitution = b2cc.Restitution;
            shapeDef.density = b2cc.Density;
        }
    }
}
void Scene::OnRuntimeStop() {
    b2DestroyWorld(*m_WorldID);
    delete m_WorldID;
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

    // Physics
    {
        constexpr float timestep = 1 / 60;
        constexpr int subStepCount = 4;

        b2World_Step(*m_WorldID, timestep, subStepCount);

        auto view = m_Registry.view<RigidBodyComponent>();

        for (auto& e : view) {
            Entity entity = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<RigidBodyComponent>();

            b2Vec2 position = b2Body_GetPosition(*rb2d.RuntimeBody);
            b2Rot rotation = b2Body_GetRotation(*rb2d.RuntimeBody);

            transform.Translation.x = position.x;
            transform.Translation.y = position.y;

            transform.Rotation.z = b2Rot_GetAngle(rotation);
        }
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

template <>
void Scene::OnComponentAdded<RigidBodyComponent>(Entity entity, RigidBodyComponent& component) {}

template <>
void Scene::OnComponentAdded<BoxColiderComponent>(Entity entity, BoxColiderComponent& component) {}

} // namespace zirconium
