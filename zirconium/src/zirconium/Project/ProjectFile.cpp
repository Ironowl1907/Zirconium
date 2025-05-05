#include "ProjectFile.h"
#include "zrpch.h"

#include <memory>
#include <yaml-cpp/yaml.h>

#include "ProjectSerializer.h"

namespace zirconium {

Ref<Project> Project::s_CurrentProject = nullptr;

Ref<Project> Project::New(const std::filesystem::path& path) {
    s_CurrentProject = std::make_shared<Project>();
    return s_CurrentProject;
}
Ref<Project> Project::Load(const std::filesystem::path& path) {
    Ref<Project> project = std::make_shared<Project>();

    ProjectSerializer serializer(s_CurrentProject);
    if (serializer.Deserialize(path)) {
        project->m_ProjectDirectory = path.parent_path();
        s_CurrentProject = project;
        return s_CurrentProject;
    }
    return nullptr;
}

bool Project::SaveCurrent(const std::filesystem::path& path) {
    ProjectSerializer serializer(s_CurrentProject);
    if (serializer.Serialize(path)) {
        s_CurrentProject->m_ProjectDirectory = path.parent_path();
        return true;
    }
    return false;
}
} // namespace zirconium
