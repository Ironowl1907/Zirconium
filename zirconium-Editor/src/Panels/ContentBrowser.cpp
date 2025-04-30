#include "ContentBrowser.h"
#include "zrpch.h"

#include <cstring>
#include <cwchar>
#include <filesystem>
#include <imgui.h>

namespace zirconium {

ContentBrowserPannel::ContentBrowserPannel() {}

void ContentBrowserPannel::Init() {
    m_FileIcon = Texture2DLibrary::Get()->Add("zirconium-Editor/res/editorImg/FileIcon.png");
    m_FolderIcon = Texture2DLibrary::Get()->Add("zirconium-Editor/res/editorImg/FolderIcon.png");
}

static void FileTree(const std::filesystem::path& path) {
    for (auto& p : std::filesystem::directory_iterator(path)) {
        auto relativePath = std::filesystem::relative(p.path(), path);
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

void ContentBrowserPannel::OnImGuiRender(const std::string& path) {
    ImGui::Begin("Content Browser");

    if (m_CurrentDirectory != std::filesystem::path("./")) {
        if (ImGui::Button("..")) {
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        }
    }

    static float padding = 16.0f;
    static float thumbnailSize = 128.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
        const auto& path = directoryEntry.path();
        std::string filenameString = path.filename().string();

        ImGui::PushID(filenameString.c_str());
        Ref<Texture2D> icon = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton("Icon", (ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
        if (ImGui::BeginDragDropSource()) {
            std::filesystem::path relativePath(path);
            std::string pathStr = relativePath.string();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr.c_str(), (pathStr.length() + 1) * sizeof(char));

            ImGui::PopStyleColor();
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (directoryEntry.is_directory())
                    m_CurrentDirectory /= path.filename();
            }
            ImGui::TextWrapped("%s", filenameString.c_str());

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);

        ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
        ImGui::SliderFloat("Padding", &padding, 0, 32);


        ImGui::PopID();
        ImGui::PopStyleColor();
        ImGui::End();
    }
}
} // namespace zirconium
