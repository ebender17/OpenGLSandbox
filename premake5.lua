-- OpenGL-Sandbox
workspace "OpenGLSandbox"
	architecture "x64"
	startproject "OpenGLSandbox"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to OpenGL-Core
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb_image"] = "vendor/stb_image"
IncludeDir["tiny_gltf"] = "vendor/tiny_gltf"

-- Projects
group "Dependencies"
	include "OpenGLCore/vendor/GLFW"
	include "OpenGLCore/vendor/Glad"
	include "OpenGLCore/vendor/imgui"
group ""

include "OpenGLCore"
include "OpenGLSandbox"