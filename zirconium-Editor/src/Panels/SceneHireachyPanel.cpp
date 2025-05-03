#include "SceneHireachyPanel.h"
#include "zirconium/Scripting/ScriptSystem.h"
#include "zirconium/Utils/PlatformUtils.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <cfloat>
#include <cstring>
#include <filesystem>
#include <memory>

namespace zirconium {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
    m_SelectionContext = {};
}
void SceneHierarchyPanel::OnImGuiRender() {
    ImGui::Begin("Scene Hierarchy");

    if (m_Context) {
        auto view = m_Context->m_Registry.view<TagComponent>();
        for (auto ent : view) {
            Entity entity{ent, m_Context.get()};
            DrawEntityNode(entity);
        }
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
        m_SelectionContext = {};
    }

    ImGuiPopupFlags flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
    if (ImGui::BeginPopupContextWindow(0, flags)) {
        if (ImGui::MenuItem("Create Empty Entity")) {
            m_Context->CreateEntity("Empty Entity");
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_SelectionContext.Entity && m_SelectionContext.Component != Components::None) {
        DrawComponents(m_SelectionContext);
    }
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext.Entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", tag.c_str());

    if (ImGui::BeginPopupContextItem()) {

        if (!entity.HasComponent<CameraComponent>())
            if (ImGui::MenuItem("Camera")) {
                entity.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
        if (!entity.HasComponent<TransformComponent>())
            if (ImGui::MenuItem("Transform")) {
                entity.AddComponent<TransformComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<SpriteRendererComponent>())
            if (ImGui::MenuItem("Sprite Renderer")) {
                entity.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<CircleRendererComponent>())
            if (ImGui::MenuItem("Circle Renderer")) {
                entity.AddComponent<CircleRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<RigidBodyComponent>())
            if (ImGui::MenuItem("Rigidbody 2D")) {
                entity.AddComponent<RigidBodyComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<BoxColiderComponent>())
            if (ImGui::MenuItem("Box Colider 2D")) {
                entity.AddComponent<BoxColiderComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<CircleColiderComponent>())
            if (ImGui::MenuItem("Circle Colider 2D")) {
                entity.AddComponent<CircleColiderComponent>();
                ImGui::CloseCurrentPopup();
            }

        if (!entity.HasComponent<LuaScriptComponent>())
            if (ImGui::MenuItem("Lua Script")) {
                entity.AddComponent<LuaScriptComponent>();
                ImGui::CloseCurrentPopup();
            }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::MenuItem("Delete Entity")) {
            m_Context->DeleteEntity(entity);
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked()) {
        m_SelectionContext.Entity = entity;
        m_SelectionContext.Component = Components::None;
    }

    if (opened) {
        struct ComponentInfo {
            const char* name;
            Components type;
            bool hasComponent;
        };

        // clang-format off
        ComponentInfo components[] = {
            { "Tag", Components::TagComponent, entity.HasComponent<TagComponent>() },
            { "SpriteRenderer", Components::SpriteRendererComponent, entity.HasComponent<SpriteRendererComponent>() },
            { "Camera", Components::CameraComponent, entity.HasComponent<CameraComponent>() },
            { "RigidBody", Components::RigidBodyComponent, entity.HasComponent<RigidBodyComponent>() },
            { "BoxColider", Components::BoxColiderComponent, entity.HasComponent<BoxColiderComponent>() },
            { "CircleRenderer", Components::CircleRendererComponent, entity.HasComponent<CircleRendererComponent>() },
            { "CircleColider", Components::CircleColiderComponent, entity.HasComponent<CircleColiderComponent>() },
            { "Transform", Components::TransformComponent, entity.HasComponent<TransformComponent>() },
            { "LuaScript", Components::LuaScriptComponent, entity.HasComponent<LuaScriptComponent>() }
        };
        // clang-format on

        for (const auto& component : components) {
            if (!component.hasComponent)
                continue;

            bool isSelected = (m_SelectionContext.Component == component.type && m_SelectionContext.Entity == entity);

            if (ImGui::Selectable(component.name, isSelected)) {
                m_SelectionContext.Component = component.type;
                m_SelectionContext.Entity = entity;
            }
            bool removeComponent = false;
            if (ImGui::BeginPopupContextItem()) {

                if (ImGui::MenuItem("Delete Component")) {
                    removeComponent = true;
                }
                ImGui::EndPopup();
            }
            if (removeComponent) {
                switch (component.type) {
                case Components::None:
                    ZR_ASSERT(false, "Removing 'None' Component");
                    break;
                case Components::IDComponent:
                    entity.RemoveComponent<IDComponent>();
                    break;
                case Components::TagComponent:
                    entity.RemoveComponent<TagComponent>();
                    break;
                case Components::RigidBodyComponent:
                    entity.RemoveComponent<RigidBodyComponent>();
                    break;
                case Components::SpriteRendererComponent:
                    entity.RemoveComponent<SpriteRendererComponent>();
                    break;
                case Components::TransformComponent:
                    entity.RemoveComponent<TransformComponent>();
                    break;
                case Components::CircleColiderComponent:
                    entity.RemoveComponent<CircleColiderComponent>();
                    break;
                case Components::CircleRendererComponent:
                    entity.RemoveComponent<CircleRendererComponent>();
                    break;
                case Components::BoxColiderComponent:
                    entity.RemoveComponent<BoxColiderComponent>();
                    break;
                case Components::LuaScriptComponent:
                    entity.RemoveComponent<LuaScriptComponent>();
                    break;
                case Components::CameraComponent:
                    entity.RemoveComponent<CameraComponent>();
                    break;
                }
            }
        }

        ImGui::TreePop();
    }
}

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
                            bool allowNegative = true, float columnWidth = 100.0f) {

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, allowNegative ? 0.0f : FLT_MAX);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, allowNegative ? 0.0f : FLT_MAX);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, allowNegative ? 0.0f : FLT_MAX);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& lable, Entity entity, UIFunction uiFunction) {

    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                       ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth |
                                       ImGuiTreeNodeFlags_FramePadding;

    if (entity.HasComponent<T>()) {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((const void*)typeid(T).hash_code(), treeNodeFlags, "%s", lable.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
            ImGui::OpenPopup("ComponentSettings");
        }
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove Component")) {
                removeComponent = true;
            }

            ImGui::EndPopup();
        }

        if (open) {
            uiFunction(component);
            ImGui::TreePop();
        }
        if (removeComponent)
            entity.RemoveComponent<T>();
    }
};

void SceneHierarchyPanel::DrawComponents(EntityComponentSelection& selection) {
    static bool BrowsingScript = false;
    static bool CreatingScript = false;
    static bool BrowseOnCreatingScript = false;

    switch (selection.Component) {
    case Components::None:
        break;

    case Components::TagComponent:
        DrawComponent<TagComponent>("Tag", selection.Entity, [](auto& component) {
            auto& tag = component.Tag;

            static char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        });
        break;
    case Components::TransformComponent:
        DrawComponent<TransformComponent>("Transform", selection.Entity, [](auto& component) {
            auto& tc = component;
            DrawVec3Control("Translation", tc.Translation);
            glm::vec3 rotation = glm::degrees(tc.Rotation);
            DrawVec3Control("Rotation", rotation);
            tc.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", tc.Scale, 1.0f, false);
        });
        break;

    case Components::SpriteRendererComponent:
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", selection.Entity, [this](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
            ImGui::SliderFloat("Tiling Factor", &component.TilingFactor, 0.0f, 100.f);

            auto& pathName = m_SelectionContext.Entity.GetComponent<SpriteRendererComponent>().Texture->GetPath();
            char buffer[128];
            std::strcpy(buffer, pathName.c_str());

            ImGui::SetNextItemWidth(200);
            if (ImGui::InputText("Texture Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::filesystem::path path(buffer);
                if (std::filesystem::exists(path))
                    m_SelectionContext.Entity.GetComponent<SpriteRendererComponent>().Texture =
                        Texture2DLibrary::Get()->Add(path);
                else {
                    ZR_CORE_ERROR("Invalid path for loading texture! {}", path.c_str());
                }
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const char* data = reinterpret_cast<const char*>(payload->Data);
                    const int dataSize = payload->DataSize;

                    std::filesystem::path path(data);
                    m_SelectionContext.Entity.GetComponent<SpriteRendererComponent>().Texture =
                        Texture2DLibrary::Get()->Add(path);
                }
                ImGui::EndDragDropTarget();
            }
        });
        break;

    case Components::LuaScriptComponent:
        DrawComponent<LuaScriptComponent>("Script", selection.Entity, [this](auto& component) {
            auto& pathName = m_SelectionContext.Entity.GetComponent<LuaScriptComponent>().ScriptPath;
            char buffer[128];
            std::strcpy(buffer, pathName.c_str());

            ImGui::SetNextItemWidth(200);
            if (ImGui::InputText("Script Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::filesystem::path path(buffer);
                if (std::filesystem::exists(path)) {
                    if (ScriptingSystem::Get()->LoadScript2Entity(m_SelectionContext.Entity, path)) {
                        m_SelectionContext.Entity.GetComponent<LuaScriptComponent>().ScriptPath = path;
                    } else {
                        ZR_ASSERT(false, "Error Loading script!");
                    }
                }
            }

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const char* data = reinterpret_cast<const char*>(payload->Data);
                    const int dataSize = payload->DataSize;

                    std::filesystem::path path(data);
                    if (ScriptingSystem::Get()->LoadScript2Entity(m_SelectionContext.Entity, path))
                        m_SelectionContext.Entity.GetComponent<LuaScriptComponent>().ScriptPath = path;
                    else
                        ZR_ASSERT(false, "Error Loading script!");
                }
                ImGui::EndDragDropTarget();
            }

            if (ImGui::Button("Browse"))
                BrowsingScript = true;

            ImGui::SameLine();

            if (ImGui::Button("Create Script"))
                CreatingScript = true;

            if (BrowsingScript) {
                std::string path;
                if (FileDialogs::OpenFile(path, ".lua")) {
                    if (!ScriptingSystem::Get()->LoadScript2Entity(m_SelectionContext.Entity,
                                                                   std::filesystem::path(path)) &&
                        !path.empty())
                        ZR_ASSERT(false, "Error Loading script!");

                    BrowsingScript = false;
                }
            }

            if (CreatingScript) {
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Create Script", 0,
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize);

                ImGui::Text("Create a Script");

                char buffer[128];
                static std::string path = "./";

                std::strcpy(buffer, path.c_str());
                if (ImGui::InputText("Script Path", buffer, sizeof(buffer))) {
                }
                ImGui::SameLine();

                if (ImGui::Button("Browse")) {
                    BrowseOnCreatingScript = true;
                    // path = "./LuaScript.lua";
                }

                if (BrowseOnCreatingScript) {
                    if (FileDialogs::SaveFile(path, ".lua")) {
                        BrowseOnCreatingScript = false;
                    }
                }

                ImGui::Separator();

                if (ImGui::Button("Cancel"))
                    CreatingScript = false;
                ImGui::SameLine();
                if (ImGui::Button("Create Script")) {
                    if (ScriptingSystem::Get()->LoadScript2Entity(m_SelectionContext.Entity, path)) {
                        m_SelectionContext.Entity.GetComponent<LuaScriptComponent>().ScriptPath = path;
                        CopyFiles("zirconium/src/zirconium/Scripting/ScriptTemplates/BasicTemplate.lua", path);
                    } else
                        ZR_ASSERT(false, "Error Loading script!");
                    CreatingScript = false;
                }

                ImGui::End();
            }
        });
        break;

    case Components::CircleRendererComponent:
        DrawComponent<CircleRendererComponent>("Circle Renderer", selection.Entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
            ImGui::SliderFloat("Thickness", &component.Thickness, 0.0f, 1.f);
            ImGui::SliderFloat("Fade", &component.Fade, 0.025f, 1.f);
        });
        break;

    case Components::CameraComponent:
        DrawComponent<CameraComponent>("Camera", selection.Entity, [](auto& component) {
            auto& cameraComponent = component;
            auto& camera = cameraComponent.Camera;

            ImGui::Checkbox("Primary", &cameraComponent.Primary);

            const char* projectionTypeStrings[] = {"Perspective", "OrthoGraphic"};
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (int i = 0; i < 2; i++) {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
                if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
                    camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));
                float orthoNear = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near Clip", &orthoNear))
                    camera.SetPerspectiveNearClip(orthoNear);
                float orthoFar = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far Clip", &orthoFar))
                    camera.SetPerspectiveFarClip(orthoFar);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                float orthoSize = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &orthoSize))
                    camera.SetOrthographicSize(orthoSize);
                float orthoNear = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near Clip", &orthoNear))
                    camera.SetOrthographicNearClip(orthoNear);
                float orthoFar = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far Clip", &orthoFar))
                    camera.SetOrthographicFarClip(orthoFar);

                ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
            }
        });
        break;

    case Components::RigidBodyComponent:
        DrawComponent<RigidBodyComponent>("Rigidbody 2D", selection.Entity, [](auto& component) {
            const char* bodyTypeStrings[] = {"Static", "Dynamic", "Kinematic"};
            const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

            if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
                for (int i = 0; i < 3; i++) {

                    bool isSelected = currentBodyTypeString == bodyTypeStrings[i];

                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
                        currentBodyTypeString = bodyTypeStrings[i];
                        component.Type = (RigidBodyComponent::BodyType)i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
            ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
        });
        break;

    case Components::BoxColiderComponent:
        DrawComponent<BoxColiderComponent>("Box Colider 2D", selection.Entity, [](auto& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
            ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
            ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Resititution", &component.Restitution, 0.01f, 0.0f);
        });
        break;

    case Components::CircleColiderComponent:
        DrawComponent<CircleColiderComponent>("Circle Colider 2D", selection.Entity, [](auto& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
            ImGui::DragFloat("Radius", &component.Radius);
            ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Resititution", &component.Restitution, 0.01f, 0.0f);
        });
        break;

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
    }
}
} // namespace zirconium
