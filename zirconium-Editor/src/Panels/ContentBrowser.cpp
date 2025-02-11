#include "ContentBrowser.h"
#include "zrpch.h"

#include <filesystem>
#include <imgui.h>

namespace zirconium {

// Delete these asap
const std::filesystem::path s_AssetsDirectory = ".";

ContentBrowserPannel::ContentBrowserPannel() {
    m_CurrentDirecory = s_AssetsDirectory;
}

void ContentBrowserPannel::OnImGuiRender() {
    ImGui::Begin("Content Browser");

    ZR_CORE_WARN("Currentdir: {}", m_CurrentDirecory.c_str());
    ZR_CORE_WARN("AssetsDir: {}", s_AssetsDirectory.c_str());

    if (m_CurrentDirecory != s_AssetsDirectory) {
        if (ImGui::Button("..")) {
            m_CurrentDirecory = m_CurrentDirecory.parent_path();
        }
    }

    for (auto& p : std::filesystem::directory_iterator(m_CurrentDirecory)) {

        std::string path = p.path();
        auto relativepath = std::filesystem::relative(path, s_AssetsDirectory);
        std::string filename = relativepath.filename();

        if (p.is_directory()) {
            if (ImGui::Button(filename.c_str())) {
                m_CurrentDirecory /= p.path().filename();
            }
        } else {
            ImGui::Text("%s", filename.c_str());
        }
    }

    ImGui::End();
}
} // namespace zirconium
