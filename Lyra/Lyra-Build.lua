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
       defines { "LR_DEBUG", "LR_ENABLE_ASSERTS" }
       links { "Vendor/assimp/lib/Debug/assimp-vc143-mtd.lib" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "LR_RELEASE" }
       links { "Vendor/assimp/lib/Release/assimp-vc143-mt.lib" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "LR_DIST" }
       links { "Vendor/assimp/lib/Release/assimp-vc143-mt.lib" }
       runtime "Release"
       optimize "On"
       symbols "Off"

   filter "configurations:*"
        postbuildcommands {
            '{MKDIR} ../Binaries/%{OutputDir}/App',
            '{COPYDIR} ./Vendor/assimp/bin/%{cfg.buildcfg}/ ../Binaries/%{OutputDir}/App/'
        }
