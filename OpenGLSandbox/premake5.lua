project "OpenGLSandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "../OpenGLCore/src",
        "../OpenGLCore/%{IncludeDir.GLFW}",
        "../OpenGLCore/%{IncludeDir.Glad}",
        "../OpenGLCore/%{IncludeDir.glm}",
        "../OpenGLCore/%{IncludeDir.ImGui}",
        "../OpenGLCore/%{IncludeDir.tiny_gltf}",
        "../OpenGLCore/vendor",
        "../OpenGLCore/vendor/spdlog/include",
    }

    links
    {
        "OpenGLCore"
    }

    dependson { "OpenGLCore" }

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