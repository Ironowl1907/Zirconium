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

void ContentBrowserPannel::OnImGuiRender() {
    ImGui::Begin("Content Browser");

    if (m_CurrentDirectory != s_AssetsDirectory) {
        if (ImGui::Button("..")) {
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
        }
    }

    for (auto& p : std::filesystem::directory_iterator(m_CurrentDirectory)) {
        auto relativePath = std::filesystem::relative(p.path(), s_AssetsDirectory);
        std::string filename = relativePath.filename().string();

        if (p.is_directory()) {
            if (ImGui::Button(filename.c_str())) {
                m_CurrentDirectory /= p.path().filename();
            }
        } else {
            ImGui::Text("%s", filename.c_str());
        }
    }

    ImGui::End();
}
} // namespace zirconium
