#include "ProjectSerializer.h"
#include "zrpch.h"
#include <yaml-cpp/yaml.h>

namespace zirconium {

bool ProjectSerializer::Serialize(const std::filesystem::path& path) {
    const auto& config = m_Project->GetConfig();

    YAML::Emitter out;
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Project" << YAML::Value;
        {
            out << YAML::BeginMap;
            out << YAML::Key << "Name" << YAML::Value << config->Name;
            out << YAML::Key << "StartScene" << YAML::Value << config->StartScene.string();
            out << YAML::Key << "AssetPath" << YAML::Value << config->AssetPath.string();
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
    }

    std::ofstream fout(path);
    fout << out.c_str();

    return true;
}
bool ProjectSerializer::Deserialize(const std::filesystem::path& path) {
    auto& config = *m_Project->GetConfig();

    YAML::Node data;
    try {
        data = YAML::LoadFile(path.string());
    } catch (YAML::ParserException e) {
        ZR_CORE_ERROR("Failed to load project file '{0}'\n     {1}", path.string(), e.what());
        return false;
    }

    auto projectNode = data["Project"];
    if (!projectNode)
        return false;

    config.Name = projectNode["Name"].as<std::string>();
    config.StartScene = projectNode["StartScene"].as<std::string>();
    config.AssetPath = projectNode["AssetPath"].as<std::string>();
    return true;
}

bool ProjectSerializer::SerializeRuntime(const std::filesystem::path& path) {
    ZR_CORE_ASSERT(false, "SerializeRuntime not Implemented!");
    return false;
}
bool ProjectSerializer::DeserializeRuntime(const std::filesystem::path& path) {
    ZR_CORE_ASSERT(false, "DeserializeRuntime not Implemented!");
    return false;
}

} // namespace zirconium
