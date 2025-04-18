#include "ContentBrowser.h"
#include "zrpch.h"

#include <cstring>
#include <cwchar>
#include <filesystem>
#include <imgui.h>

namespace zirconium {

// Delete these asap
const std::filesystem::path s_AssetsDirectory = ".";

ContentBrowserPannel::ContentBrowserPannel() {
    m_CurrentDirectory = s_AssetsDirectory;
}

static void FileTree(const std::filesystem::path& path) {
    for (auto& p : std::filesystem::directory_iterator(path)) {
        auto relativePath = std::filesystem::relative(p.path(), s_AssetsDirectory);
        std::string filename = relativePath.filename().string();

        if (p.is_directory()) {
            if (ImGui::TreeNode(filename.c_str())) {
                FileTree(p.path());
                ImGui::TreePop();
            }
        } else {
            ImGui::TreeNodeEx(filename.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
            if (ImGui::BeginDragDropSource()) {
                const char* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (strlen(itemPath) + 1) * sizeof(char),
                                          ImGuiCond_Once);
                ImGui::EndDragDropSource();
            }
        }
    }
}

void ContentBrowserPannel::OnImGuiRender() {
    ImGui::Begin("Content Browser");


    FileTree(s_AssetsDirectory);
    ImGui::End();
}
} // namespace zirconium
