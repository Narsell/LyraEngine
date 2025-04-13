OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDir = {}

-- This path is relative to Lyra
IncludeDir["GLFW"] = "Vendor/GLFW/include"
IncludeDir["Glad"] = "Vendor/glad/include"
IncludeDir["ImGui"] = "Vendor/imgui"
IncludeDir["glm"] = "Vendor/glm"
IncludeDir["spdlog"] = "Vendor/spdlog/include"
IncludeDir["stb_image"] = "Vendor/stb_image"
IncludeDir["assimp"] = "Vendor/assimp/include"

workspace "Lyra"
   architecture "x64"
   configurations { "Debug", "Release", "Dist"}
   startproject "LyraEditor"

   flags
	{
		"MultiProcessorCompile"
	}

   group "Dependencies"
   include "Lyra/Vendor/GLFW"
   include "Lyra/Vendor/Glad"
   include "Lyra/Vendor/imgui"
   group ""

   group "Engine"
   include "Lyra/Lyra-Build.lua"
   include "LyraEditor/Editor-Build.lua"
   group ""

   include "Sandbox/Sandbox-Build.lua"
