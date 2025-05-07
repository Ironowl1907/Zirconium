#pragma once

#include "core.h"
#include "zirconium/Renderer/Texture.h"
#include <filesystem>

namespace zirconium {
class ContentBrowserPannel {

public:
    ContentBrowserPannel();

    void Init();
    void OnImGuiRender();
    void SetCurrentDirectory(const std::filesystem::path& path) {
        m_CurrentDirectory = path;
    }

private:
    std::filesystem::path m_CurrentDirectory = "./";
    Ref<Texture2D> m_FolderIcon;
    Ref<Texture2D> m_FileIcon;
};
} // namespace zirconium
