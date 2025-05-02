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
IncludeDir["assimp_conf"] = "Vendor/assimp/_config_headers"

workspace "Lyra"
   architecture "x64"
   configurations { "Debug", "Release", "Dist"}
   startproject "LyraEditor"

   flags
	{
		"MultiProcessorCompile"
	}

   -- Common configurations to all projects --
   project "*"
      staticruntime "On"

      buildoptions { "/utf-8" }

      filter "configurations:Debug"
      defines { "LR_DEBUG", "LR_ENABLE_ASSERTS" }
      runtime "Debug"
      symbols "On"

      filter "configurations:Release"
      defines { "LR_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

      filter "configurations:Dist"
      defines { "LR_DIST" }
      runtime "Release"
      optimize "Full"
      symbols "Off"

      filter "system:windows"
         systemversion "latest"
         defines
         {
            "LR_PLATFORM_WINDOWS"
         }

   group "Dependencies"
      include "Lyra/Vendor/GLFW/GLFWBuild.lua"
      include "Lyra/Vendor/Glad/GladBuild.lua"
      include "Lyra/Vendor/imgui/ImGuiBuild.lua"
      include "Lyra/Vendor/assimp/AssimpBuild.lua"
   group ""

   group "Engine"
      include "Lyra/LyraBuild.lua"
      group ""

   include "LyraEditor/EditorBuild.lua"
   include "Sandbox/SandboxBuild.lua"
