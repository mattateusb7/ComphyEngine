workspace "Comphi"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "Sandbox"

-- VARS --
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories
IncludeDir = {};
--Libs
IncludeDir["spdlog"] = "Comphi/vendor/spdlog/include";
IncludeDir["glm"] = "Comphi/vendor/glm";
--Projs
IncludeDir["GLFW"] = "Comphi/vendor/GLFW/include";
IncludeDir["Glad"] = "Comphi/vendor/Glad/include";
IncludeDir["Imgui"] = "Comphi/vendor/imgui";

-- END VARS --

--premake5 file for vendor submodules
include "Comphi/vendor"

project "Comphi"
    location "Comphi"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "cphipch.h"
    pchsource "Comphi/src/cphipch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        --Libs
        "%{prj.name}/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        --Projs
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Imgui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "Imgui",
        "opengl32.lib"
    }

    filter "system:windows"     
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM",
            "CPHI_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        defines 
        {
            "CPHI_DEBUG", 
            "CPHI_ENABLE_ASSERTS"
        }
		runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "CPHI_RELEASE"
		runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "CPHI_DIST"
		runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.Imgui}",
        "Comphi/src",
        "%{prj.name}/src"
    }

    links
    {
        "Comphi"
    }

    filter "system:windows"     
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM"
        }

    filter "configurations:Debug"
        defines "CPHI_DEBUG"
		runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "CPHI_RELEASE"
		runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "CPHI_DIST"
		runtime "Release"
        optimize "on"