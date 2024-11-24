workspace("MyProject")
warnings("Extra")
architecture("x64")
configurations({ "Debug", "Release", "Dist" })
location("build")

newaction({
	trigger = "clean",
	description = "Clean the build and bin directories",
	execute = function()
		print("Cleaning build directories...")
		os.rmdir("build")
		os.rmdir("bin")
		os.rmdir("bin-int")
		print("Done.")
	end,
})

-- Include directories
IncludeDir = {}
IncludeDir["GLFW"] = "./zirconium/vendor/glfw/include/"

include("./zirconium/vendor/glfw/")

-- Project for zirconium shared library
project("zirconium")
kind("SharedLib") -- Create .so shared library
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")
pchheader("./zirconium/zrpch.h")
files({ "zirconium/zirconium/**.h", "zirconium/zirconium/**.cpp" })
includedirs({ "zirconium/vendor/spdlog/include", IncludeDir["GLFW"] })
links({ "GLFW", "GL", "m", "dl", "X11", "pthread" }) -- Link with pthread

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
includedirs({ "zirconium/src", "./zirconium/vendor/spdlog/include" })
links({ "zirconium" }) -- Link with the zirconium shared library

-- Linux-specific settings
filter("system:linux")
buildoptions({ "-pthread" })
linkoptions({ "-pthread" })

-- Debug Configuration
filter("configurations:Debug")
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("Debug")

-- Release Configuration
filter("configurations:Release")
defines({ "ZIR_RELEASE", "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("On")

-- Distribution Configuration
filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On")
