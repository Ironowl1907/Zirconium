#include "Components.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "zrpch.h"

#include "zirconium/Renderer/Renderer2D.h"
#include "zirconium/Scene/Components.h"
#include "zirconium/Scene/Entity.h"
#include "zirconium/Scripting/ScriptSystem.h"
#include <cstdint>
#include <glm/trigonometric.hpp>
#include <memory>

#include "Scene.h"
#include "box2d/box2d.h"

#include "NativeScript.h"
#include "box2d/id.h"

namespace zirconium {

class CameraScript : public ScriptableEntity {
public:
    void OnCreate() override {
        ZR_CORE_TRACE("Created Camera script component");
    }

    void OnUpdate(float dt) override {
        auto& transform = GetComponent<TransformComponent>();
        transform.Translation.x += 1.0f * dt; // Example movement
        std::cout << "Transform.x " << &transform.Translation.x << " Entityid: " << (uint64_t)m_Entity << '\n';
    }
};

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

template <typename Component>
static void CopyComponent(entt::registry& dst, entt::registry& src,
                          const std::unordered_map<UUID, entt::entity>& enttMap) {

    auto view = src.view<Component>();
    for (auto e : view) {
        UUID uuid = src.get<IDComponent>(e).ID;
        ZR_ASSERT(enttMap.find(uuid) != enttMap.end(), "");
        entt::entity dstEnttID = enttMap.at(uuid);

        auto& component = src.get<Component>(e);
        dst.emplace_or_replace<Component>(dstEnttID, component);
    }
}

template <typename Component>
static void CopyComponentIfExists(Entity dst, Entity src) {
    if (src.HasComponent<Component>())
        dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
}

void Scene::DuplicateEntity(Entity entity) {
    Entity newEntity = CreateEntity(entity.GetTag());

    CopyComponentIfExists<TransformComponent>(newEntity, entity);
    CopyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
    CopyComponentIfExists<CameraComponent>(newEntity, entity);
    CopyComponentIfExists<RigidBodyComponent>(newEntity, entity);
    CopyComponentIfExists<BoxColiderComponent>(newEntity, entity);
    CopyComponentIfExists<CircleRendererComponent>(newEntity, entity);
    CopyComponentIfExists<CircleColiderComponent>(newEntity, entity);
    CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
    CopyComponentIfExists<LuaScriptComponent>(newEntity, entity);
}
Ref<Scene> Scene::Copy(Ref<Scene> other) {
    Ref<Scene> newScene = std::make_shared<Scene>();

    auto& srcSceneRegistry = other->m_Registry;
    auto& dstSceneRegistry = newScene->m_Registry;
    newScene->m_ViewportWidth = other->m_ViewportWidth;
    newScene->m_ViewportHeight = other->m_ViewportHeight;

    std::unordered_map<UUID, entt::entity> enttMap;

    // Create entities new scene
    auto idView = srcSceneRegistry.view<IDComponent>();
    for (auto e : idView) {
        UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
        const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
        Entity newEntity = newScene->CreateEntityWithID(uuid, name);
        enttMap[uuid] = (entt::entity)newEntity;
    }

    CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<RigidBodyComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<BoxColiderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<CircleColiderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
    CopyComponent<LuaScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

    return newScene;
}

Scene::Scene() {
    m_Registry = entt::registry();
};

Scene::~Scene() {}

void Scene::OnUpdateEditor(TimeStep delta, EditorCamera& camera) {
    RenderScene(camera);
}

void Scene::OnPhysicsInit() {

    b2WorldDef worldDefinition = b2DefaultWorldDef();
    worldDefinition.gravity = (b2Vec2){0.0f, -10.0f};

    m_WorldID = new b2WorldId;
    *m_WorldID = b2CreateWorld(&worldDefinition);
    ZR_ASSERT(m_WorldID != nullptr, "World ID is null");

    auto view = m_Registry.view<RigidBodyComponent>();
    for (auto e : view) {
        Entity entity = {e, this};
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<RigidBodyComponent>();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = ZirconiumRigidBody2DToB2DRigidBodyType(rb2d.Type);
        bodyDef.position = {transform.Translation.x, transform.Translation.y};
        bodyDef.rotation = b2MakeRot(transform.Rotation.z);

        b2BodyId bodyId = b2CreateBody(*m_WorldID, &bodyDef);
        b2Body_SetFixedRotation(bodyId, rb2d.FixedRotation);

        rb2d.RuntimeBody = bodyId;

        if (entity.HasComponent<BoxColiderComponent>()) {
            auto& b2cc = entity.GetComponent<BoxColiderComponent>();

            b2Polygon colider = b2MakeBox(b2cc.Size.x * transform.Scale.x, b2cc.Size.y * transform.Scale.y);

            b2ShapeDef shapeDef = b2DefaultShapeDef();
            b2SurfaceMaterial material = b2DefaultSurfaceMaterial();

            ZR_ASSERT(b2cc.Density, "Density can't be 0");
            shapeDef.density = b2cc.Density;
            material.friction = b2cc.Friction;
            material.restitution = b2cc.Restitution;

            shapeDef.material = material;

            b2CreatePolygonShape(bodyId, &shapeDef, &colider);
        }

        if (entity.HasComponent<CircleColiderComponent>()) {
            auto& cc2b = entity.GetComponent<CircleColiderComponent>();

            b2Circle colider = {{cc2b.Offset.x, cc2b.Offset.y}, transform.Scale.x * cc2b.Radius};

            b2ShapeDef shapeDef = b2DefaultShapeDef();
            b2SurfaceMaterial material = b2DefaultSurfaceMaterial();

            ZR_ASSERT(cc2b.Density, "Density can't be 0");
            shapeDef.density = cc2b.Density;
            material.friction = cc2b.Friction;
            material.restitution = cc2b.Restitution;

            shapeDef.material = material;

            b2CreateCircleShape(bodyId, &shapeDef, &colider);
        }
    }
}

void Scene::OnPhysicsShutdown() {
    b2DestroyWorld(*m_WorldID);
    delete m_WorldID;
}

bool Scene::OnScriptsInit() {
    ScriptingSystem* sc = ScriptingSystem::Get();

    sc->Init(this);
    return sc->InitScripts();
}

bool Scene::OnRuntimeStart() {
    OnPhysicsInit();
    return OnScriptsInit();
}

void Scene::OnRuntimeStop() {
    OnPhysicsShutdown();
}

void Scene::OnUpdateRuntime(TimeStep delta) {
    // Update Native Scripts
    {
        auto view = m_Registry.view<NativeScriptComponent>();
        for (auto entity : view) {
            auto& scriptComponent = m_Registry.get<NativeScriptComponent>(entity);
            if (!scriptComponent.Instance) {
                scriptComponent.Instance = scriptComponent.InstantiateScript();
                scriptComponent.Instance->m_Entity = Entity(entity, this);
                scriptComponent.Instance->OnCreate();
            }

            scriptComponent.Instance->OnUpdate(delta);
        }
    }
    // Update Lua Scripts
    {
        ScriptingSystem::Get()->UpdateScripts(delta);
    }

    // Physics
    {

        constexpr float timestep = 1.0f / 60.0f;
        constexpr int subStepCount = 8;

        b2World_Step(*m_WorldID, timestep, subStepCount);

        auto view = m_Registry.view<RigidBodyComponent>();

        for (auto& e : view) {
            Entity entity = {e, this};
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<RigidBodyComponent>();

            b2Vec2 position = b2Body_GetPosition(rb2d.RuntimeBody);
            b2Rot rotation = b2Body_GetRotation(rb2d.RuntimeBody);

            transform.Translation.x = position.x;
            transform.Translation.y = position.y;

            transform.Rotation.z = b2Rot_GetAngle(rotation);
        }
    }

    // Render Sprites

    Entity cameraEntity = GetMainCameraEntity();

    auto mainCamera = cameraEntity.GetComponent<CameraComponent>().Camera;
    auto mainTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();

    Renderer2D::BeginScene(mainCamera, mainTransform);
    // Quads
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(), sprite, (uint32_t)entity);
        }
    }

    // Circles
    {
        auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
        for (auto entity : view) {
            const auto& [transform, crc] = view.get<TransformComponent, CircleRendererComponent>(entity);
            Renderer2D::DrawCircle(transform.GetTransform(), crc.Color, (uint32_t)entity, crc.Thickness, crc.Fade);
        }
    }
    Renderer2D::EndScene();
}

void Scene::OnSimulationStart() {
    OnPhysicsInit();
}

void Scene::OnSimulationStop() {
    OnPhysicsShutdown();
}

void Scene::OnUpdateSimulation(TimeStep delta, EditorCamera& camera) {

    // Physics

    constexpr float timestep = 1.0f / 60.0f;
    constexpr int subStepCount = 8;

    b2World_Step(*m_WorldID, timestep, subStepCount);

    auto view = m_Registry.view<RigidBodyComponent>();

    for (auto& e : view) {
        Entity entity = {e, this};
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<RigidBodyComponent>();

        b2Vec2 position = b2Body_GetPosition(rb2d.RuntimeBody);
        b2Rot rotation = b2Body_GetRotation(rb2d.RuntimeBody);

        transform.Translation.x = position.x;
        transform.Translation.y = position.y;

        transform.Rotation.z = b2Rot_GetAngle(rotation);
    }

    // Render
    RenderScene(camera);
}

Entity Scene::CreateEntity(const std::string& name) {
    return CreateEntityWithID(UUID(), name);
}

Entity Scene::CreateEntityWithID(const uint64_t id, const std::string& name) {
    entt::entity ent = m_Registry.create();
    Entity entity = Entity(ent, this);
    entity.AddComponent<IDComponent>(id);
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

void Scene::RenderScene(EditorCamera& camera) {
    Renderer2D::BeginScene(camera);

    // Draw Sprites
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group) {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawSprite(transform.GetTransform(), sprite, (uint64_t)entity);
        }
    }

    // Draw Circles
    {
        auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
        for (auto entity : view) {
            const auto& [transform, crc] = view.get<TransformComponent, CircleRendererComponent>(entity);
            Renderer2D::DrawCircle(transform.GetTransform(), crc.Color, (uint32_t)entity, crc.Thickness, crc.Fade);
        }
    }

    Renderer2D::EndScene();
}

void Scene::DeleteEntity(Entity entity) {
    m_Registry.destroy((entt::entity)entity);
}

Entity Scene::GetMainCameraEntity() {
    auto view = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view) {
        const auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
        if (camera.Primary)
            return {entity, this};
    }
    ZR_ASSERT(false, "Scene doesn't have main camera!");
    return Entity();
}

} // namespace zirconium
