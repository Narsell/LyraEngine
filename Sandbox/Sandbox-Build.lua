project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "On"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

	  -- Include Lyra
	  "../Lyra/Source",
      "../Lyra/Source/Lyra",
      "../Lyra/Source/Lyra/Core",
      "../Lyra/Source/Lyra/Engine",
      "../Lyra/Source/Lyra/Scene",
      "../Lyra/Source/Lyra/Tools",
      "../Lyra/%{IncludeDir.spdlog}",
      "../Lyra/%{IncludeDir.glm}",
      "../Lyra/%{IncludeDir.ImGui}",
   }

   links
   {
      "Lyra"
   }

   buildoptions { "/utf-8" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
        systemversion "latest"
        defines
        {
            "LR_PLATFORM_WINDOWS"
        }

   filter "configurations:Debug"
       defines { "LR_DEBUG" }
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

    filter "configurations:*"
       postbuildcommands {
           '{COPYDIR} ../LyraEditor/Assets ../Binaries/%{OutputDir}/%{prj.name}/Assets'
       }
