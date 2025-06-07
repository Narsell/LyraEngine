project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"

   files { "Source/**.h", "Source/**.cpp", }
   
   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")
   debugdir "%{wks.location}"

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
      "Lyra",
      "GLFW",
      "Glad",
      "ImGui",
      "assimp",
   }

    filter "configurations:*"
       postbuildcommands {
           '{COPYDIR} %{wks.location}/Assets ../Binaries/%{OutputDir}/%{prj.name}/Assets'
       }
