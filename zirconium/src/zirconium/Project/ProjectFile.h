#pragma once

#include <filesystem>
#include <map>
namespace zirconium {

class ProjectFile {
public:
    ProjectFile() {}
    ProjectFile(std::string& name, const std::string& defScene = "", const std::string& selfPath = "")
        : m_ProjectName(name)
        , m_DefaultScene(defScene)
        , m_SelfPath(selfPath) {}

    void Load(const std::filesystem::path& path);

    static void SerializeProject(const ProjectFile& project, std::filesystem::path& path);
    void Serialize(std::filesystem::path& path);

private:
private:
    std::string m_ProjectName;
    std::string m_DefaultScene = "";
    std::filesystem::path m_SelfPath;
};

} // namespace zirconium
