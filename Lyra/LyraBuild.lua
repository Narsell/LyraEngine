project "Lyra"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"

   files
   {
      "Source/**.h",
      "Source/**.hpp",
      "Source/**.cpp",
      "Source/**.c",
      -- Include glm files
      "Vendor/glm/glm/**.hpp",
      "Vendor/glm/glm/**.inl",
      -- Include stb files
      "Vendor/stb_image/**.h",
      "Vendor/stb_image/**.cpp",
   }

   includedirs
   {
      "Source",
      "Source/Lyra",
      "Source/Lyra/Core",
      "Source/Lyra/Engine",
      "Source/Lyra/Scene",
      "Source/Lyra/Tools",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.stb_image}",
      "%{IncludeDir.assimp}",
      "%{IncludeDir.assimp_conf}"
   }

   links
   {
      "GLFW",
      "Glad",
      "ImGui",
      "assimp",
      "opengl32.lib",
   }

   defines
   {
    "_CRT_SECURE_NO_WARNINGS", -- stb generates some warnings about some C runtime unsafe methods ¯\_(ツ)_/¯
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   pchheader "lrpch.h"
   pchsource "Source/lrpch.cpp"