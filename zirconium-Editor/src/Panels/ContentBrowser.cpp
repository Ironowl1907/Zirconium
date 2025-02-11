#include "ContentBrowser.h"
#include "zrpch.h"

#include <filesystem>
#include <imgui.h>

namespace zirconium {

// Delete these asap
const std::filesystem::path s_AssetsDirectory = ".";

ContentBrowserPannel::ContentBrowserPannel() {
    m_CurrentDirectory = s_AssetsDirectory;
}

static void FileTree(std::filesystem::path path) {

    for (auto& p : std::filesystem::directory_iterator(path)) {
        auto relativePath = std::filesystem::relative(p.path(), s_AssetsDirectory);
        std::string filename = relativePath.filename().string();

        if (p.is_directory()) {
            if (ImGui::TreeNode(filename.c_str())) {
                FileTree(path / filename);
                ImGui::TreePop();
            }
        } else {
            ImGui::TreeNodeEx(filename.c_str(), ImGuiTreeNodeFlags_Leaf);
            ImGui::TreePop();
        }
    }
    return;
}

void ContentBrowserPannel::OnImGuiRender() {
    ImGui::Begin("Content Browser");

    FileTree(s_AssetsDirectory);
    ImGui::End();
}
} // namespace zirconium
