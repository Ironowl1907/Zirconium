#include "ProjectFile.h"
#include "zrpch.h"

#include <fstream>
#include <sstream>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/yaml.h>

namespace zirconium {

void ProjectFile::Load(std::filesystem::path& path) {}
void ProjectFile::Serialize(std::filesystem::path& path) {
    YAML::Emitter out;
    out << YAML::Key << "ProjectName" << YAML::Value << m_ProjectName;

    out << YAML::Key << "Scenes" << YAML::Value << YAML::BeginMap;
    for (auto& [name, path] : m_Scenes) {
        out << YAML::Key << name << YAML::Value << path.c_str();
    }
    out << YAML::EndMap;
}

} // namespace zirconium
