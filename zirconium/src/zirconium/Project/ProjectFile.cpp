#include "ProjectFile.h"
#include "zrpch.h"

#include <filesystem>
#include <memory>
#include <yaml-cpp/yaml.h>

#include "ProjectSerializer.h"

namespace zirconium {

Ref<Project> Project::s_CurrentProject = nullptr;
ProjectConfig Project::s_ProjectConfig("Untitled");
std::filesystem::path Project::s_ResolutionFilePath = "";

Ref<Project> Project::New() {
    s_CurrentProject = std::make_shared<Project>();
    s_ProjectConfig.AssetPath = ".";
    return s_CurrentProject;
}
Ref<Project> Project::New(const std::filesystem::path& path, const ProjectConfig config) {
    s_CurrentProject = std::make_shared<Project>();
    s_ResolutionFilePath = path;
    s_ProjectConfig.AssetPath = config.AssetPath;
    s_ProjectConfig.StartScene = config.StartScene;
    s_ProjectConfig.Name = config.Name;
    return s_CurrentProject;
}
Ref<Project> Project::Load(const std::filesystem::path& path) {
    Ref<Project> project = std::make_shared<Project>();

    ProjectSerializer serializer(s_CurrentProject);
    if (serializer.Deserialize(path)) {
        project->s_ResolutionFilePath = path.parent_path();
        s_CurrentProject = project;
        return s_CurrentProject;
    }
    return nullptr;
}

bool Project::SaveCurrent(const std::filesystem::path& path) {
    ZR_CORE_ASSERT(s_CurrentProject, "Current Project is Null!");
    ProjectSerializer serializer(s_CurrentProject);
    if (serializer.Serialize(path)) {
        s_CurrentProject->s_ResolutionFilePath = path.parent_path();
        return true;
    }
    return false;
}
} // namespace zirconium
