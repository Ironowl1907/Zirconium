#pragma once

#include <filesystem>
#include "zirconium/Renderer/Texture.h"
#include "core.h"

namespace zirconium {
class ContentBrowserPannel {

public:
    ContentBrowserPannel();

    void Init();
    void OnImGuiRender(const std::string& path);

private:
    std::filesystem::path m_CurrentDirectory = "./";
    Ref<Texture2D> m_FolderIcon;
    Ref<Texture2D> m_FileIcon;
};
} // namespace zirconium
