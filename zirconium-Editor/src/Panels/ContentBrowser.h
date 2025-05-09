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
    void SetRootDirectory(const std::filesystem::path& path) {
        m_RootDirectory = path;
    }

private:
    std::filesystem::path m_CurrentDirectory;
    std::filesystem::path m_RootDirectory;
    Ref<Texture2D> m_FolderIcon;
    Ref<Texture2D> m_FileIcon;
};
} // namespace zirconium
