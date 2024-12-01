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
IncludeDir["Glad"] = "./zirconium/vendor/glad/include/"
IncludeDir["ImGui"] = "./zirconium/vendor/imgui/"

include("./zirconium/vendor/glad/")
include("./zirconium/vendor/glfw/")
include("./zirconium/vendor/imgui/")

-- Project for zirconium shared library
project("zirconium")
kind("SharedLib") -- Create .so shared library
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")
pchheader("./zirconium/zrpch.h")
files({ "zirconium/zirconium/**.h", "zirconium/zirconium/**.cpp" })
includedirs({
	"./zirconium/",
	"./zirconium/zirconium/",
	"zirconium/vendor/spdlog/include",
	IncludeDir["GLFW"],
	IncludeDir["Glad"],
	IncludeDir["ImGui"],
})
links({ "Glad", "GLFW", "ImGui", "GL", "m", "dl", "X11", "pthread" })

pic("On") -- Enable Position Independent Code for shared libraries

-- Linux-specific settings
filter("system:linux")
buildoptions({ "-pthread" })
linkoptions({ "-pthread" })

-- Debug Configuration
filter("configurations:Debug")
defines({ "ZIR_DEBUG" })
optimize("Debug") -- Disable optimizations for Debug builds

-- Release Configuration
filter("configurations:Release")
defines({ "ZIR_RELEASE" })
optimize("On") -- Enable optimizations for Release builds

-- Distribution Configuration
filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On") -- Optimizations for Distribution

-- Project for sandbox executable
project("sandbox")
kind("ConsoleApp") -- Create executable
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "./sandbox/src/**.cpp", "./sandbox/src/**.h" })
includedirs({ "zirconium/src", "zirconium/", "./zirconium/vendor/spdlog/include" })
links({ "zirconium", "Glad" }) -- Link with the zirconium shared library

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
