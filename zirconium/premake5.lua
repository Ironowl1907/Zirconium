project("zirconium")
kind("staticLib")
language("C++")
cppdialect "C++20"
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")

pchheader("./zirconium/src/zrpch.h")
pchsource("./zirconium/src/zrpch.cpp")

files({
    "src/**.h",
    "src/**.cpp",

    "vendor/stb_image/**.h",
    "vendor/stb_image/**.cpp",

    "vendor/glm/glm/**.hpp",
})

includedirs({
    "./src",
    "./src/zirconium",
    IncludeDir["GLFW"],
    IncludeDir["Glad"],
    IncludeDir["ImGui"],
    IncludeDir["glm"],
    IncludeDir["stb_image"],
    IncludeDir["entt"],
    IncludeDir["ImGuiFD"],
    IncludeDir["yaml-cpp"],
    IncludeDir["shaderc"],
    IncludeDir["spirv-cross"],
    IncludeDir["Box2D"],
})
-- links({ "yaml-cpp", "spdLog", "fmt", "Glad", "GLFW", "ImGui", "GL", "m", "dl", "X11", "pthread", "ImGuiFileDialog" })



links({
    "shaderc-combined",
    "GLFW",
    "Glad",
    "SpdLog",
    "ImGui",
    "yaml-cpp",
    "spirv-cross",
    "opengl32.lib",
    "Box2D",
})

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
buildoptions { "-ggdb" }
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
