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

    // m_Context->m_Registry.each([&](auto entityID) {
    //     Entity entity{entity, m_Context.get()};

    //     auto& tag = entity.GetComponent<TagComponent>().Tag;
    //     ImGui::Text("%s", tag);
    // });
    //
    auto view = m_Context->m_Registry.view<TagComponent>();

    for (auto ent : view) {
        Entity entity{ent, m_Context.get()};

        auto& tag = entity.GetComponent<TagComponent>().Tag;
        ImGui::BulletText("%s", tag.c_str());
    }
    ImGui::End();
};
} // namespace zirconium
