workspace("Zirconium")
startproject("zirconium-Editor")
-- warnings("Extra")
architecture("x64")
configurations({ "Debug", "Release", "Profile-Debug", "Profile-Release", "Dist" })
location("build")


newaction({
    trigger = "clean",
    description = "Clean the build and bin directories recursively",
    execute = function()
        print("Cleaning build directories...")

        -- Ensure platform-specific compatibility
        local deleteCommand
        if os.host() == "windows" then
            deleteCommand = "rmdir /S /Q"
        else
            deleteCommand = "rm -rf"
        end

        -- Remove the directories
        os.execute(deleteCommand .. " build")
        os.execute(deleteCommand .. " bin")
        os.execute(deleteCommand .. " bin-int")

        print("Done.")
    end,
})

-- Include directories
IncludeDir = {}
IncludeDir["GLFW"] = "./zirconium/vendor/glfw/include/"
IncludeDir["spdlog"] = "./zirconium/vendor/spdlog/include/"
IncludeDir["Glad"] = "./zirconium/vendor/glad/include/"
IncludeDir["ImGui"] = "./zirconium/vendor/imgui/"
IncludeDir["glm"] = "./zirconium/vendor/glm/"
IncludeDir["stb_image"] = "./zirconium/vendor/stb_image/"
IncludeDir["entt"] = "./zirconium/vendor/entt/src/entt"
IncludeDir["yaml-cpp"] = "./zirconium/vendor/yaml-cpp/include"
IncludeDir["ImGuiFileDialog"] = "./zirconium/vendor/ImGuiFileDialog"

include("./zirconium/vendor/glad/")
include("./zirconium/vendor/glfw/")
include("./zirconium/vendor/imgui/")
include("./zirconium/vendor/spdlog/")
include("./zirconium/vendor/yaml-cpp/")
include("./zirconium/vendor/ImGuiFileDialog/")

-- Project for zirconium static library
project("zirconium")
kind("staticLib")
language("C++")
cppdialect "C++20"
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")
pchheader("./zirconium/zrpch.h")
files({
    "zirconium/zirconium/**.h",
    "zirconium/zirconium/**.cpp",
    "./zirconium/vendor/stb_image/**.h",
    "./zirconium/vendor/stb_image/**.cpp"
})
includedirs({
    "./zirconium/",
    IncludeDir["GLFW"],
    IncludeDir["Glad"],
    IncludeDir["ImGui"],
    IncludeDir["glm"],
    IncludeDir["stb_image"],
    IncludeDir["entt"],
    IncludeDir["yaml-cpp"],
    IncludeDir["ImGuiFileDialog"],
})
links({ "yaml-cpp", "spdLog", "fmt", "Glad", "GLFW", "ImGui", "GL", "m", "dl", "X11", "pthread", "ImGuiFileDialog" })

pic("On") -- Enable Position Independent Code for shared libraries (optional for StaticLib)

defines {
    "YAML_CPP_STATIC_DEFINE"
}
-- Linux-specific settings
filter("system:linux")
buildoptions({ "-pthread" })
linkoptions({ "-pthread" })

-- Debug Configuration
filter("configurations:Debug")
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS", "GLFW_INCLUDE_NONE" })
optimize("Debug")

-- Release Configuration
filter("configurations:Release")
defines({ "ZIR_RELEASE", "ZR_ENABLE_ASSERTS" })
optimize("On")

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
optimize("On")

-- Add warnings for all compilers
filter "toolset:gcc or clang"
buildoptions { "-Wall", "-Wextra", "-Werror", "-Wno-unused-parameter" } -- Enable all warnings for GCC/Clang, but suppress unused parameter warnings (remove after finish)

filter "toolset:msc"                                                    -- For MSVC (Microsoft compiler)
buildoptions { "/W4", "/WX" }                                           -- Enable most warnings and treat warnings as errors (remove after finish)


project("sandbox")
kind("ConsoleApp") -- Changed from ConsoleApp to StaticLib
language("C++")
cppdialect "C++20"
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "./sandbox/src/**.cpp", "./sandbox/src/**.h" })
includedirs({
    "zirconium/",
    "zirconium/vendor",
    IncludeDir["glm"],
    IncludeDir["ImGui"],
    IncludeDir["entt"],
    IncludeDir["ImGuiFileDialog"],
})
links({ "yaml-cpp", "zirconium", "fmt", "spdLog", "Glad", "ImGui", "GLFW", "GL", "m", "dl", "X11", "pthread",
    "ImGuiFileDialog" })

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


project("zirconium-Editor")
kind("ConsoleApp") -- Changed from ConsoleApp to StaticLib
language("C++")
cppdialect "C++20"
staticruntime "on"
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium-Editor")
files({ "./zirconium-Editor/src/**.cpp", "./zirconium-Editor/src/**.h" })

includedirs({
    "zirconium/",
    "zirconium/vendor",
    IncludeDir["glm"],
    IncludeDir["spdlog"],
    IncludeDir["entt"],
    IncludeDir["yaml-cpp"],
    IncludeDir["ImGuiFileDialog"],
})

libdirs { "./zirconium/vendor/yaml-cpp/bin/Release" }
links({ "fmt", "zirconium", "spdLog", "Glad", "ImGui", "GLFW", "GL", "m", "dl", "X11", "pthread", "yaml-cpp",
    "ImGuiFileDialog" })

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
