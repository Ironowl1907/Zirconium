#include "SceneSerializer.h"
#include "Components.h"
#include "Entity.h"
#include "SceneCamera.h"
#include "zrpch.h"

#include <fstream>
#include <sstream>
#include <string>
#include <yaml-cpp/yaml.h>

namespace YAML {

template <>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        return node;
    }
    static bool decode(const Node node, glm::vec3& rhs) {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec2> {
    static Node encode(const glm::vec2& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        return node;
    }
    static bool decode(const Node node, glm::vec2& rhs) {
        if (!node.IsSequence() || node.size() != 2)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        return node;
    }
    static bool decode(const Node node, glm::vec4& rhs) {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};

} // namespace YAML

namespace zirconium {

static std::string B2Type2String(RigidBodyComponent::BodyType type) {
    switch (type) {
    case RigidBodyComponent::BodyType::Static:
        return "Static";
    case RigidBodyComponent::BodyType::Dynamic:
        return "Dynamic";
    case RigidBodyComponent::BodyType::Kinematics:
        return "Kinematics";
    }

    ZR_ASSERT(false, "No such type");
    return "";
}

static RigidBodyComponent::BodyType B2TypeFromString(std::string& type) {
    if (type == "Static")
        return RigidBodyComponent::BodyType::Static;
    if (type == "Dynamic")
        return RigidBodyComponent::BodyType::Dynamic;
    if (type == "Kinematic")
        return RigidBodyComponent::BodyType::Kinematics;

    ZR_ASSERT(false, "No such type {}", type);
    return RigidBodyComponent::BodyType::Kinematics;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {

    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {

    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {

    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

SceneSerializer::SceneSerializer(const Ref<Scene> scene)
    : m_Scene(scene) {}

static void SerializeEntity(YAML::Emitter& out, Entity entity) {
    ZR_ASSERT(entity.HasComponent<IDComponent>(), "Entity without UUID!");

    out << YAML::BeginMap;
    out << YAML::Key << "Entity";
    out << YAML::Value << entity.GetID();

    if (entity.HasComponent<TagComponent>()) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;

        auto& tag = entity.GetComponent<TagComponent>().Tag;
        out << YAML::Key << "Tag" << YAML::Value << tag.c_str();

        out << YAML::EndMap;
    }

    if (entity.HasComponent<TransformComponent>()) {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;

        auto& tc = entity.GetComponent<TransformComponent>();
        out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
        out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
        out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

        out << YAML::EndMap;
    }
    if (entity.HasComponent<CameraComponent>()) {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        auto& cameraComponent = entity.GetComponent<CameraComponent>();
        auto& camera = cameraComponent.Camera;

        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
        out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
        out << YAML::Key << "ProjectionNear" << YAML::Value << camera.GetPerspectiveNearClip();
        out << YAML::Key << "ProjectionFar" << YAML::Value << camera.GetPerspectiveFarClip();
        out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
        out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
        out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
        out << YAML::EndMap;

        out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<SpriteRendererComponent>()) {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<RigidBodyComponent>()) {
        out << YAML::Key << "RigidBodyComponent";
        out << YAML::BeginMap;

        auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
        out << YAML::Key << "FixedRotation" << YAML::Value << rigidBodyComponent.FixedRotation;
        out << YAML::Key << "Type" << YAML::Value << B2Type2String(rigidBodyComponent.Type);

        out << YAML::EndMap;
    }

    if (entity.HasComponent<BoxColiderComponent>()) {
        out << YAML::Key << "BoxColiderComponent";
        out << YAML::BeginMap;

        auto& boxColiderComponent = entity.GetComponent<BoxColiderComponent>();
        out << YAML::Key << "Offset" << YAML::Value << boxColiderComponent.Offset;
        out << YAML::Key << "Size" << YAML::Value << boxColiderComponent.Size;

        out << YAML::Key << "Density" << YAML::Value << boxColiderComponent.Density;
        out << YAML::Key << "Friction" << YAML::Value << boxColiderComponent.Friction;
        out << YAML::Key << "Restitution" << YAML::Value << boxColiderComponent.Restitution;

        out << YAML::EndMap;
    }

    if (entity.HasComponent<CircleRendererComponent>()) {
        out << YAML::Key << "CircleRendererComponent";
        out << YAML::BeginMap;

        auto& crc = entity.GetComponent<CircleRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << crc.Color;
        out << YAML::Key << "Thickness" << YAML::Value << crc.Thickness;
        out << YAML::Key << "Fade" << YAML::Value << crc.Fade;

        out << YAML::EndMap;
    }
    out << YAML::EndMap;
}

void SceneSerializer::Serialize(const std::string& filepath) {
    ZR_CORE_TRACE("Starting scene serialization in path: {}", filepath);
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

    m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID) {
        Entity entity = {entityID, m_Scene.get()};
        if (!entity)
            return;
        SerializeEntity(out, entity);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}
void SceneSerializer::SerializeRuntime(const std::string& filepath) {
    ZR_CORE_ASSERT(false, "Unimplemented");
}

bool SceneSerializer::Deserialize(const std::string& filepath) {
    std::ifstream stream(filepath);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"])
        return false;

    std::string sceneName = data["Scene"].as<std::string>();
    ZR_CORE_TRACE("Deserializing scene {0}", sceneName);

    auto entities = data["Entities"];
    if (entities) {
        for (auto entity : entities) {

            uint64_t uuid = entity["Entity"].as<uint64_t>();

            std::string name;
            auto tagComponent = entity["TagComponent"];
            if (tagComponent) {
                name = tagComponent["Tag"].as<std::string>();
            }
            ZR_CORE_TRACE("Deserialized Tag component '{0}' with uuid {1}", name, uuid);

            Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, name);

            auto transformComponent = entity["TransformComponent"];
            if (transformComponent) {
                // Entities should always have transforms
                auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                ZR_CORE_TRACE("Deserialized Transform component with uuid {0}", uuid);
            } else
                ZR_ASSERT(false, "Enity '{}' doesn't have a transform component", name);

            auto cameraComponent = entity["CameraComponent"];
            if (cameraComponent) {
                auto& cc = deserializedEntity.AddComponent<CameraComponent>();

                auto cameraProps = cameraComponent["Camera"];
                cc.Camera.SetViewportSize(m_Scene->m_ViewportWidth, m_Scene->m_ViewportHeight);
                cc.Camera.SetProjectionType((SceneCamera::ProjectionType)(cameraProps["ProjectionType"].as<int>()),
                                            false);

                cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
                cc.Camera.SetPerspectiveNearClip(cameraProps["ProjectionNear"].as<float>());
                cc.Camera.SetPerspectiveFarClip(cameraProps["ProjectionFar"].as<float>());

                cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                cc.Primary = cameraComponent["Primary"].as<bool>();
                cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

                ZR_CORE_TRACE("Deserialized Camera component with uuid {0}", uuid);
            }

            auto spriteRendererComponent = entity["SpriteRendererComponent"];
            if (spriteRendererComponent) {
                auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
                ZR_CORE_TRACE("Deserialized Sprite Renderer component with uuid {0}", uuid);
            }

            auto circleRenererComponent = entity["CircleRendererComponent"];
            if (circleRenererComponent) {
                auto& src = deserializedEntity.AddComponent<CircleRendererComponent>();
                src.Color = circleRenererComponent["Color"].as<glm::vec4>();
                src.Thickness = circleRenererComponent["Thickness"].as<float>();
                src.Fade = circleRenererComponent["Fade"].as<float>();
                ZR_CORE_TRACE("Deserialized Circle Renderer component with uuid {0}", uuid);
            }

            auto rigidBodyComponent = entity["RigidBodyComponent"];
            if (rigidBodyComponent) {
                auto& src = deserializedEntity.AddComponent<RigidBodyComponent>();
                std::string type = rigidBodyComponent["Type"].as<std::string>();
                src.Type = B2TypeFromString(type);
                src.FixedRotation = rigidBodyComponent["FixedRotation"].as<bool>();
                ZR_CORE_TRACE("Deserialized RigidBody component with uuid {0}", uuid);
            }

            auto boxColiderComponent = entity["BoxColiderComponent"];
            if (rigidBodyComponent) {
                auto& src = deserializedEntity.AddComponent<BoxColiderComponent>();
                src.Offset = (glm::vec2)boxColiderComponent["Offset"].as<glm::vec2>();
                src.Size = (glm::vec2)boxColiderComponent["Size"].as<glm::vec2>();

                src.Density = boxColiderComponent["Density"].as<float>();
                src.Friction = boxColiderComponent["Friction"].as<float>();
                src.Restitution = boxColiderComponent["Restitution"].as<float>();
                ZR_CORE_TRACE("Deserialized Sprite component with uuid {0}", uuid);
            }
        }
    }
    return true;
}
bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
    ZR_CORE_ASSERT(false, "Unimplemented");
}
} // namespace zirconium
