project "Lyra"
   kind "SharedLib"
   language "C++"

   files { "Source/**.h", "Source/**.hpp", "Source/**.cpp", "Source/**.c" }

   includedirs
   {
      "Source"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       cppdialect "C++20"
       staticruntime "On"
       systemversion "latest"
       defines
       {
          "LR_PLATFORM_WINDOWS",
          "LR_BUILD_DLL"
       }

       postbuildcommands
       {
        ("{COPY} %{cfg.buildtarget.relpath} ../Binaries/" .. OutputDir ..  "/App")
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
       defines { "HZ_DIST" }
       runtime "Release"
       optimize "On"