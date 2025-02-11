#pragma once

#include <filesystem>

namespace zirconium {
class ContentBrowserPannel {

public:
    ContentBrowserPannel();

    void OnImGuiRender();
private:
std::filesystem::path m_CurrentDirecory;
};
} // namespace zirconium
