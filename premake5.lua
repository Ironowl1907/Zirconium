workspace("MyProject")
configurations({ "Debug", "Release" })
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

-- Project for zirconium shared library
project("zirconium")
kind("SharedLib") -- Create .so shared library
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")
files({ "zirconium/zirconium/**.h", "zirconium/zirconium/**.cpp" })
pic("On") -- Enable Position Independent Code for shared libraries

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")

-- Project for sandbox executable
project("sandbox")
kind("ConsoleApp") -- Create executable
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/sandbox")
files({ "sandbox/src/SandboxApp.cpp" })
includedirs({ "zirconium/src" }) -- Include zirconium headers
links({ "zirconium" }) -- Link with the zirconium shared library

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")
