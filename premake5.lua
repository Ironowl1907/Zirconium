include "Dependencies.lua"

workspace "zirconium"
architecture "x86_64"
startproject "zirconium-editor"

configurations
{
    "Debug",
    "Release",
    "Dist"
}


libdirs {
    "zirconium/vendor/shaderc/libshaderc",
    "zirconium/vendor/spirv-cross/",

    "./zirconium/vendor/yaml-cpp/bin/Release"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "zirconium/vendor/glfw"
include "zirconium/vendor/imgui"
include "zirconium/vendor/glad"
include "zirconium/vendor/ImGuiFileDialog"
include "zirconium/vendor/spdlog"
include "zirconium/vendor/yaml-cpp"

group ""

include "zirconium"
-- include "Sandbox"
include "zirconium-Editor"
