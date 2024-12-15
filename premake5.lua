workspace("MyProject")
-- warnings("Extra")
architecture("x64")
configurations({ "Debug", "Release", "Dist" })
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

include("./zirconium/vendor/glad/")
include("./zirconium/vendor/glfw/")
include("./zirconium/vendor/imgui/")
include("./zirconium/vendor/spdlog/")

-- Project for zirconium static library
project("zirconium")
kind("staticLib") -- Create .so shared library
language("C++")
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
    "./zirconium/zirconium/",
    IncludeDir["GLFW"],
    IncludeDir["Glad"],
    IncludeDir["ImGui"],
    IncludeDir["glm"],
    IncludeDir["stb_image"],
})
links({ "spdLog", "Glad", "GLFW", "ImGui", "GL", "m", "dl", "X11", "pthread" })

pic("On") -- Enable Position Independent Code for shared libraries (optional for StaticLib)

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
defines({ "ZIR_RELEASE", "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("On")

-- Distribution Configuration
filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On")

-- Project for sandbox static library
project("sandbox")
kind("ConsoleApp") -- Changed from ConsoleApp to StaticLib
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "./sandbox/src/**.cpp", "./sandbox/src/**.h" })
includedirs({
    "zirconium/src",
    "zirconium/",
    "./zirconium/zirconium/",
    IncludeDir["glm"],
    IncludeDir["ImGui"],
})
links({ "zirconium", "spdLog", "Glad", "ImGui", "GLFW", "GL", "m", "dl", "X11", "pthread" })

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
defines({ "ZIR_RELEASE", "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("On")

-- Distribution Configuration
filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On")
