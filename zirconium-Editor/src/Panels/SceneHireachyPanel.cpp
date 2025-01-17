#include "SceneHireachyPanel.h"
#include "zirconium/scene/Components.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

namespace zirconium {

SceneHirearchyPanel::SceneHirearchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHirearchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
}
void SceneHirearchyPanel::OnImGuiRender() {
    ImGui::Begin("Scene Hiererchy");

    auto view = m_Context->m_Registry.view<TagComponent>();
    for (auto ent : view) {
        Entity entity{ent, m_Context.get()};
        DrawEntityNode(entity);
    }
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
        m_SelectionContext = {};
    }

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectionContext) {
        DrawComponents(m_SelectionContext);
    }
    ImGui::End();
}

void SceneHirearchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", tag.c_str());
    if (ImGui::IsItemClicked()) {
        m_SelectionContext = entity;
    }
    if (opened) {
        ImGui::TreePop();
    }
}

void SceneHirearchyPanel::DrawComponents(Entity ent) {
    if (ent.HasComponent<TagComponent>()) {
        auto& tag = ent.GetComponent<TagComponent>().Tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, tag.c_str());

        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

    if (ent.HasComponent<SpriteRendererComponent>()) {
        if (ImGui::TreeNodeEx((const void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                              "Sprite")) {

            auto& sprite = ent.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Color", glm::value_ptr (sprite.Color));

            ImGui::TreePop();
        }
    }

    if (ent.HasComponent<TransformComponent>()) {
        if (ImGui::TreeNodeEx((const void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                              "Transform")) {

            auto& transform = ent.GetComponent<TransformComponent>().Transform;
            ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

            ImGui::TreePop();
        }
    }

    if (ent.HasComponent<CameraComponent>()) {
        if (ImGui::TreeNodeEx((const void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                              "Camera")) {

            auto& cameraComponent = ent.GetComponent<CameraComponent>();
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

            ImGui::TreePop();
        }
    }
}
} // namespace zirconium
