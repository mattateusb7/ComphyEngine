workspace "Comphi"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Comphi"
    location "Comphi"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "cphipch.h"
    pchsource "Comphi/src/cphipch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
		"%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"     
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CPHI_WINDOWS_PLATFORM",
            "CPHI_BUILD_DLL" 
        }

        postbuildcommands
        {
            ("{MKDIR} ../bin/".. outputdir .."/Sandbox"),
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/".. outputdir .."/Sandbox")
        }

        cleancommands { "" }
    
    filter "configurations:Debug"
        defines "CPHY_DEBUG"
        symbols "On"
        
    filter "configurations:Release"
        defines "CPHY_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CPHY_DIST"
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
        defines "CPHY_DEBUG"
        symbols "On"
        
    filter "configurations:Release"
        defines "CPHY_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CPHY_DIST"
        optimize "On"