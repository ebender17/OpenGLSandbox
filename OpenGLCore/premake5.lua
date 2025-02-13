project "OpenGLCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "glpch.h"
    pchsource "src/glpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/tiny_gltf/**.h",
        "vendor/tiny_glft/**.hpp",
        "vendor/tiny_gltf/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.tiny_gltf}"
    }

    links 
    { 
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    -- Apply /utf-8 only for Visual Studio
    filter "action:vs*"
        buildoptions { "/utf-8" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "OPENGLCORE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "OPENGLCORE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "OPENGLCORE_RELEASE"
        runtime "Release"
        optimize "on"