workspace "Lyra"
   architecture "x64"
   configurations { "Debug", "Release", "Dist"}
   startproject "App"

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["GLFW"] = "Lyra/Vendor/GLFW/include"

group "Dependencies"
   include "Lyra/Vendor/GLFW"
group ""

group "Engine"
	include "Lyra/Lyra-Build.lua"
group ""

include "App/App-Build.lua"