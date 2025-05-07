#pragma once

#include "core.h"
#include <filesystem>

namespace zirconium {

struct ProjectConfig {
    ProjectConfig(const char* name)
        : Name(name) {}

    std::string Name;

    std::filesystem::path StartScene;
    std::filesystem::path AssetPath;
};

class Project {
public:
    static std::string GetProjectName() {
        ZR_CORE_ASSERT(s_CurrentProject, "Current Project is Null!");
        return s_CurrentProject->s_ProjectConfig.Name;
    }
    static std::filesystem::path GetStarterScene() {
        ZR_CORE_ASSERT(s_CurrentProject, "Current Project is Null!");
        return s_CurrentProject->s_ProjectConfig.StartScene;
    }
    static std::filesystem::path GetAssetPath() {
        ZR_CORE_ASSERT(s_CurrentProject, "Current Project is Null!");
        return s_CurrentProject->s_ProjectConfig.AssetPath;
    }

    static Ref<Project> GetActive() {
        return s_CurrentProject;
    }

    ProjectConfig* GetConfig() {
        return &s_ProjectConfig;
    }

    static Ref<Project> New();
    static Ref<Project> New(const std::filesystem::path& path, const ProjectConfig config);
    static Ref<Project> Load(const std::filesystem::path& path);
    static bool SaveCurrent(const std::filesystem::path& path);

private:
private:
    static ProjectConfig s_ProjectConfig;
    static Ref<Project> s_CurrentProject;
    static std::filesystem::path s_ResolutionFilePath;
};

} // namespace zirconium
