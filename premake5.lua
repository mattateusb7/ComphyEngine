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
IncludeDir["GLFW"] = "Comphi/vendor/GLFW/include";
IncludeDir["Glad"] = "Comphi/vendor/Glad/include";

-- END VARS --

--premake5 file for vendor submodules
include "Comphi/vendor"

project "Comphi"
    location "Comphi"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "cphipch.h"
    pchsource "Comphi/src/cphipch.cpp"

    --vpaths 
    --{
    --    --real folders
    --    ["Comphi"] = {
    --        "Comphi/src/**.h", "Comphi/src/**.cpp"
    --    },
    --    ["Comphi/Events"] = {
    --        "Comphi/src/Comphi/Events/*.h", 
    --        "Comphi/src/Comphi/Events/*.cpp"
    --    },
    --    --virtual folders
    --    ["Comphi/Core"] = {
    --        "Comphi/src/Comphi/Core.h",
    --        "Comphi/src/Comphi/Log.cpp",
    --        "Comphi/src/Comphi/Log.h",
    --        "Comphi/src/Comphi/Window.h"
    --    },
    --    ["Comphi/Application"] = {
    --        "Comphi/src/Comphi/Application.h", 
    --        "Comphi/src/Comphi/Application.cpp",
    --        "Comphi/src/Comphi/EntryPoint.h"
    --    },
    --    ["Comphi/Application/Platform/Windows"] = {
    --        "Comphi/src/Comphi/WindowsWindow.h"
    --    }
    --}

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"     
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM",
            "CPHI_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{MKDIR} ../bin/".. outputdir .."/Sandbox"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/".. outputdir .."/Sandbox")
        }

        cleancommands { "" }
    
    filter "configurations:Debug"
        defines 
        {
            "CPHI_DEBUG", 
            "CPHI_ENABLE_ASSERTS"
        }
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "CPHI_RELEASE"
        buildoptions "/MD"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CPHI_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Comphi/vendor/spdlog/include",
        "Comphi/src",
        "%{prj.name}/src"
    }

    links
    {
        "Comphi"
    }

    filter "system:windows"     
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM"
        }

    filter "configurations:Debug"
        defines "CPHI_DEBUG"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "CPHI_RELEASE"
        buildoptions "/MD"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CPHI_DIST"
        buildoptions "/MD"
        optimize "On"