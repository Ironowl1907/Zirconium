-- premake5.lua

workspace("MyProject")
configurations({ "Debug", "Release" })
location("build") -- Where to place generated files

-- Project for zirconium shared library
project("zirconium")
kind("SharedLib") -- Create .so shared library
language("C++")
targetdir("bin/%{cfg.buildcfg}")
objdir("bin-int/%{cfg.buildcfg}/zirconium")
files({ "zirconium/src/**.cpp", "zirconium/src/**.h" })
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
files({ "sandbox/src/main.cpp" })
includedirs({ "zirconium/src" }) -- Include zirconium headers
links({ "zirconium" }) -- Link with the zirconium shared library

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")
