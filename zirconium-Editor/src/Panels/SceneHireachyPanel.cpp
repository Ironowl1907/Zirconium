#include "SceneHireachyPanel.h"
#include "zirconium/scene/Components.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace zirconium {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
    m_SelectionContext = {};
}
void SceneHierarchyPanel::OnImGuiRender() {
    bool t = true;
    ImGui::ShowDemoWindow(&t);
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
        // ZR_CORE_TRACE("Hey3");
        if (ImGui::MenuItem("Create Empty Entity")) {
            m_Context->CreateEntity("Empty Entity");
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_SelectionContext) {
        DrawComponents(m_SelectionContext);
    }
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", tag.c_str());

    if (ImGui::BeginPopupContextItem()) {

        if (ImGui::MenuItem("Delete Entity")) {
            m_Context->DeleteEntity(entity);
        }

        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked()) {
        m_SelectionContext = entity;
    }

    if (opened) {
        ImGui::TreePop();
    }
}

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
                            float columnWidth = 100.0f) {

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
    ImGui::DragFloat("##X", &values.x, 0.1f);
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
    ImGui::DragFloat("##Y", &values.y, 0.1f);
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
    ImGui::DragFloat("##Z", &values.z, 0.1f);
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

void SceneHierarchyPanel::DrawComponents(Entity entity) {

    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        static char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, tag.c_str());

        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent")) {

        if (ImGui::MenuItem("Camera")) {
            m_SelectionContext.AddComponent<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Sprite Renderer")) {
            m_SelectionContext.AddComponent<SpriteRendererComponent>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
        auto& tc = component;
        DrawVec3Control("Translation", tc.Translation);
        glm::vec3 rotation = glm::degrees(tc.Rotation);
        DrawVec3Control("Rotation", rotation);
        tc.Rotation = glm::radians(rotation);
        DrawVec3Control("Scale", tc.Scale, 1.0f);
    });

    DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component) {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
    });

    DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
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

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
}
} // namespace zirconium
