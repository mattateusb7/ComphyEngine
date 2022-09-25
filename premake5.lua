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
IncludeDir["vulkan"] = "C:/VulkanSDK/1.3.224.1/Include"; -- Make it more dynamic?
IncludeDir["vulkanLib"] = "C:/VulkanSDK/1.3.224.1/Lib"; -- Make it more dynamic?
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
	cppdialect "C++20"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "cphipch.h"
    pchsource "Comphi/src/cphipch.cpp"
    
    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }

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
        "%{IncludeDir.vulkan}",
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
        "opengl32.lib",
        "%{IncludeDir.vulkanLib}/vulkan-1.lib"
    }

    filter "system:windows"     
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM",
            "CPHI_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
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
    cppdialect "C++20"
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