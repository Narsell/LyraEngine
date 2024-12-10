project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "Off"
    systemversion "latest"

    targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../../Intermediates/" .. OutputDir .. "/%{prj.name}")

    includedirs
    {
        "include",
    }

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"