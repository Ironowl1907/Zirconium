#pragma once

#include "core.h"
#include <filesystem>

namespace zirconium {

struct ProjectConfig {
    std::string Name;

    std::filesystem::path StarterScene;
    std::filesystem::path AssetPath;
};

class Project {
public:
    static std::string GetProjectName() {
        return m_CurrentProject->m_ProjectConfig.Name;
    }
    static std::filesystem::path GetStarterScene() {
        return m_CurrentProject->m_ProjectConfig.StarterScene;
    }
    static std::filesystem::path GetAssetPath() {
        return m_CurrentProject->m_ProjectConfig.AssetPath;
    }

    static Ref<Project> GetActive() {
        return m_CurrentProject;
    }

    ProjectConfig* GetConfing() {
        return &m_ProjectConfig;
    }

    static void New(const std::filesystem::path& path);
    static void Load(const std::filesystem::path& path);
    static void SaveCurrent(const std::filesystem::path& path);

private:
private:
    ProjectConfig m_ProjectConfig;
    static Ref<Project> m_CurrentProject;
    std::filesystem::path m_ProjectDirectory;
};

} // namespace zirconium
