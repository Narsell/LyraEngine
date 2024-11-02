workspace "Lyra"
   architecture "x64"
   configurations { "Debug", "Release", "Dist"}
   startproject "App"

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
	include "Lyra/Lyra-Build.lua"
group ""

include "App/App-Build.lua"