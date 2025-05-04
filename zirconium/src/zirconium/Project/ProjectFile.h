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
        return s_CurrentProject->m_ProjectConfig.Name;
    }
    static std::filesystem::path GetStarterScene() {
        return s_CurrentProject->m_ProjectConfig.StarterScene;
    }
    static std::filesystem::path GetAssetPath() {
        return s_CurrentProject->m_ProjectConfig.AssetPath;
    }

    static Ref<Project> GetActive() {
        return s_CurrentProject;
    }

    ProjectConfig* GetConfing() {
        return &m_ProjectConfig;
    }

    static Ref<Project> New(const std::filesystem::path& path);
    static Ref<Project> Load(const std::filesystem::path& path);
    static bool SaveCurrent(const std::filesystem::path& path);

private:
private:
    ProjectConfig m_ProjectConfig;
    static Ref<Project> s_CurrentProject;
    std::filesystem::path m_ProjectDirectory;
};

} // namespace zirconium
