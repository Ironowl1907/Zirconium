workspace("MyProject")
-- warnings("Extra")
architecture("x64")
configurations({ "Debug", "Release", "dist" })
location("build") -- Where to place generated files

-- Clean configuration
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
includedirs({ "zirconium/vendor/spdlog/include", IncludeDir["GLFW"] }) -- Include zirconium headers
links({ "GLFW", "GL", "m", "dl", "X11", "pthread" })
pic("On") -- Enable Position Independent Code for shared libraries
filter("configurations:Debug")
defines({ "ZIR_DEBUG" })
optimize("On")

filter("configurations:Release")
defines({ "ZIR_RELESE" })
optimize("On")

filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On")

-- Project for sandbox executable
project("sandbox")
kind("ConsoleApp") -- Create executable
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "./sandbox/src/**.cpp", "./sandbox/src/**.h" })
includedirs({ "zirconium/src", "./zirconium/vendor/spdlog/include" }) -- Include zirconium headers
links({ "zirconium" }) -- Link with the zirconium shared library

filter("configurations:Debug")
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("On")

filter("configurations:Release")
defines({ "ZIR_RELESE", "" })
defines({ "ZIR_DEBUG", "ZR_ENABLE_ASSERTS" })
optimize("On")

filter("configurations:Dist")
defines({ "ZIR_DIST" })
optimize("On")
