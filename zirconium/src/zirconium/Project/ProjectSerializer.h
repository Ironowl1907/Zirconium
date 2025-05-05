#pragma once

#include "ProjectFile.h"
#include "core.h"
#include <filesystem>
namespace zirconium {
class ProjectSerializer {
public:
    ProjectSerializer(Ref<Project> project)
        : m_Project(project) {}
    bool Serialize(const std::filesystem::path& path);
    bool Deserialize(const std::filesystem::path& path);

    bool SerializeRuntime(const std::filesystem::path& path);
    bool DeserializeRuntime(const std::filesystem::path& path);

private:
    Ref<Project> m_Project;
};
} // namespace zirconium
