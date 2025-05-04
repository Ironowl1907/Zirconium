#include "ProjectFile.h"
#include "zrpch.h"

#include <memory>
#include <yaml-cpp/yaml.h>

namespace zirconium {

Ref<Project> Project::New(const std::filesystem::path& path) {
    s_CurrentProject = std::make_shared<Project>();
    return s_CurrentProject;
}
Ref<Project> Project::Load(const std::filesystem::path& path) {
    Ref<Project> project = std::make_shared<Project>();

    if (ProjectSerializer::Serialize(path)) {
        project->m_ProjectDirectory = path.parent_path();
        s_CurrentProject = project;
        return s_CurrentProject;
    }
    return nullptr;
}

bool Project::SaveCurrent(const std::filesystem::path& path) {
    if (ProjecSerializer::Serialize(path)) {
        s_CurrentProject->m_ProjectDirectory = path.parent_path();
        return true;
    }

    return false;
}
} // namespace zirconium
