project "App"
   kind "ConsoleApp"
   language "C++"
   staticruntime "Off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

	  -- Include Lyra
	  "../Lyra/Source",
      "../Lyra/%{IncludeDir.spdlog}",
      "../Lyra/%{IncludeDir.glm}",
   }

   links
   {
      "Lyra"
   }

   buildoptions { "/utf-8" }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
        cppdialect "C++20"
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