#include "SceneHireachyPanel.h"
#include "zirconium/scene/Components.h"

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
    ImGui::End();
}

void SceneHirearchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = entity.GetComponent<TagComponent>().Tag;

    ImGuiTreeNodeFlags flags =
        ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked()) {
        m_SelectionContext = entity;
    }
    if (opened) {
        ImGui::TreePop();
    }
}
} // namespace zirconium
