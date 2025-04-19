#pragma once

#include <filesystem>

namespace zirconium {
class ContentBrowserPannel {

public:
    ContentBrowserPannel();

    void OnImGuiRender(const std::string& path);

private:
    std::filesystem::path m_CurrentDirectory;
};
} // namespace zirconium
