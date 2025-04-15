#pragma once

#include <filesystem>
#include <map>
namespace zirconium {

class ProjectFile {
public:
    void Load(std::filesystem::path& path);
    void Serialize(std::filesystem::path& path);

private:
private:
    std::string m_DefaultScene = "";
    std::string m_ProjectName;
    std::filesystem::path m_SelfPath;
};

} // namespace zirconium
