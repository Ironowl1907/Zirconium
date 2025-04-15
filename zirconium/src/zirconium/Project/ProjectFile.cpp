#include "ProjectFile.h"
#include "zrpch.h"

#include <fstream>
#include <sstream>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/yaml.h>

namespace zirconium {

void ProjectFile::Load(std::filesystem::path& path) {
    std::ifstream stream(path);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Project"])
        ZR_ASSERT(false, "Not Compatible .zr file. Can't find project name!");

    m_ProjectName = data["ProjectName"].as<std::string>();
    m_DefaultScene = data["DefaultScene"].as<std::string>();

    m_SelfPath = path.c_str();
}

void ProjectFile::Serialize(std::filesystem::path& path) {
    YAML::Emitter out;
    out << YAML::Key << "ProjectName" << YAML::Value << m_ProjectName;
    out << YAML::Key << "DefaultScene" << YAML::Value << m_DefaultScene;
}

} // namespace zirconium
