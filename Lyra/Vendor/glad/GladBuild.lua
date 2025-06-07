project "Glad"
    kind "StaticLib"
    language "C"

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

    filter "system:linux"
        pic "On"