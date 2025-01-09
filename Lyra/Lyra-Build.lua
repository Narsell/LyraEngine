project "Lyra"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "On"

   files
   {
      "Source/**.h",
      "Source/**.hpp",
      "Source/**.cpp",
      "Source/**.c",
      -- Include glm files into Lyra project
      "Vendor/glm/glm/**.hpp",
      "Vendor/glm/glm/**.inl",
   }

   includedirs
   {
      "Source",
      "%{IncludeDir.spdlog}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.glm}",
   }

   links
   {
      "GLFW",
      "Glad",
      "ImGui",
      "opengl32.lib",
   }

   defines
   {
      "_CRT_SECURE_NO_WARNINGS"
   }

   buildoptions { "/utf-8" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   pchheader "lrpch.h"
   pchsource "Source/lrpch.cpp"

   filter "system:windows"
       systemversion "latest"
       defines
       {
          "LR_PLATFORM_WINDOWS"
       }

   filter "configurations:Debug"
       defines
       {
        "LR_DEBUG",
        "LR_ENABLE_ASSERTS"
       }
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
       optimize "On"
       symbols "Off"