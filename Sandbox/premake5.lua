project("sandbox")
kind("ConsoleApp") -- Changed from ConsoleApp to StaticLib
language("C++")
cppdialect "C++20"
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "./src/**.cpp", "./src/**.h" })

includedirs({
    "../zirconium/src",
    IncludeDir["glm"],
    IncludeDir["ImGui"],
    IncludeDir["entt"],
    IncludeDir["ImGuiFileDialog"],
})
links({
    "zirconium",
    "SpdLog",
    "GLFW",
    "ImGui",
    "Glad",
    "fmt",
})

-- Linux-specific settings
filter("system:linux")
buildoptions({ "-pthread" })
linkoptions({ "-pthread" })

-- Windows-specific settings
filter("system:windows")
systemversion "latest"

-- Debug Configuration
filter("configurations:Debug")
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS", "GLFW_INCLUDE_NONE" })
symbols("On")     -- Enable debug symbols
optimize("Debug") -- Optimize for debugging

-- Release Configuration
filter("configurations:Release")
defines({ "ZIR_RELEASE", "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
symbols("Off") -- Disable debug symbols for release
optimize("On") -- Optimize for performance

-- Profile Debug Configuration
filter("configurations:Profile-Debug")
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS", "GLFW_INCLUDE_NONE", "ZR_PROFILE" })
optimize("Debug")

-- Profile Release Configuration
filter("configurations:Profile-Release")
defines({ "ZIR_RELEASE", "ZR_ENABLE_ASSERTS", "ZR_PROFILE" })
optimize("On")


-- Distribution Configuration
filter("configurations:Dist")
defines({ "ZIR_DIST" })
symbols("Off") -- Disable debug symbols for distribution
optimize("On") -- Optimize for maximum performance
