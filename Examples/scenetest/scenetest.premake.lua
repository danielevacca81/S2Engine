-- scentest.premake.lua - SceneTest Example
-- actions: 
--     vs2022
--     gmake2 --os=linux
local vcpkg_triplet = (os.host() == "windows") and "x64-windows-static-md" or "x64-linux"
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)


sourcedir   = path.getabsolute("src")
outdir      = path.getabsolute("./.build")
sysbuilddir = "%{cfg.system}/%{cfg.buildcfg}"
s2Enginedir = path.getabsolute("../../s2Engine")

workspace "SceneTest"
    location "%{outdir}"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "SceneTest"

    -- Common flags
    flags {"MultiProcessorCompile"}
    cppdialect "C++20"
 
-- SceneTest Project
project "SceneTest"
    kind "ConsoleApp"
    location "%{outdir}"
    language "C++"
    cppdialect "C++20"
    
    targetdir ( "%{outdir}/bin/%{sysbuilddir}" ) -- i.e. bin/windows/release
    objdir    ( "%{outdir}/tmp/%{sysbuilddir}" )    

    -- Source files
    files {
        "%{sourcedir}/**.h",
        "%{sourcedir}/**.c",
        "%{sourcedir}/**.hpp",
        "%{sourcedir}/**.cpp",
    }
    
    -- additional include directories
    includedirs { 
        "%{sourcedir}/",
        vcpkg_dir .. "/include",
        s2Enginedir .. "/include/",
    }

    libdirs {
        s2Enginedir .. "/bin/%{cfg.system}/%{cfg.buildcfg}/",
    }
    
    -- Librerie comuni a tutti i sistemi
    links {
        "s2Engine",
    }
        
    defines {}

    -- Platform specific settings
    filter "system:windows"
        systemversion "latest"
    filter {}
    
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
        --flags { "LinkTimeOptimization" }
    filter {}    

    filter { "system:windows", "configurations:Debug" }
        staticruntime "Off"
        libdirs {
            vcpkg_dir .. "/debug/lib",
        }
        links{
            "imguid"
        }

    filter { "system:windows", "configurations:Release" }
        staticruntime "Off"
        defines { }
        libdirs {
            vcpkg_dir .. "/lib"
        }
        links{
            "imgui"
        }        
    filter {}



    -- post build events
    filter "system:windows"
        postbuildcommands {
            ("{COPYFILE} %{s2Enginedir}/bin/%{cfg.system}/%{cfg.buildcfg}/s2Engine.dll %{cfg.targetdir}"),
        }    
        links { 
        }
        
    filter "system:linux"
        postbuildcommands {
            ("{COPYFILE} %{s2Enginedir}/bin/%{cfg.system}/%{cfg.buildcfg}/libs2Engine.so %{cfg.targetdir}"),
        }
        linkoptions { "-Wl,-rpath='$$ORIGIN'" }
        links { 
        }
        
    filter {} -- Reset filters
