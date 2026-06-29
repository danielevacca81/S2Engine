-- scentest.premake.lua - SceneTest Example
-- actions: 
--     vs2022
--     gmake2 --os=linux
local vcpkg_triplet = "x64-windows-static"
if os.host() == "linux" then
    vcpkg_triplet = "x64-linux"
end
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)

workspace "SceneTest"
    location "."
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "SceneTest"

    outdir      = path.getabsolute("./.build")

    -- Common flags
    flags {
        "MultiProcessorCompile"
    }

    -- C++ standard
    cppdialect "C++20"

    -- Platform specific settings
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        symbols "On"
        optimize "Speed"

    filter {}

-- SceneTest Project
project "SceneTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

	targetdir ( "%{outdir}/bin/%{sysbuilddir}" ) -- i.e. bin/windows/release
	objdir    ( "%{outdir}/tmp/%{sysbuilddir}" )	

    -- Source files
    files
    {
        "src/**.h",
        "src/**.hpp",
        "src/**.cpp",
    }

    -- Include directories
    includedirs {
        "src/",
        "../../s2Engine/include/",
        vcpkg_dir .. "/include"
    }

    -- Library directories
    libdirs {
        "../../s2Engine/bin/%{cfg.system}/%{cfg.buildcfg}/",
        vcpkg_dir .. "/lib"
    }

    -- Link libraries
    links {
        "s2Engine",
        "imgui",
    }
        
    filter "system:linux"
        linkoptions { "-Wl,-rpath='$$ORIGIN'" }
    
    -- Copy s2Engine to output directory
    --postbuildcommands {
    --    ("{COPYFILE} %{wks.location}/../../s2Engine/bin/%{cfg.system}/%{cfg.buildcfg}/s2Engine.dll %{cfg.buildtarget.directory}"),
    --}

    

    filter {}
