project "App"
   kind "ConsoleApp"
   language "C++"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

	  -- Include Lyra
	  "../Lyra/Source"
   }

   links
   {
      "Lyra"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
   cppdialect "C++20"
   staticruntime "On"
   systemversion "latest"
   defines
   {
      "LR_PLATFORM_WINDOWS"
   }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "HZ_DIST" }
       runtime "Release"
       optimize "On"