-- test.premake.lua

local vcpkg_triplet = "x64-windows-static"
if os.host() == "linux" then
    vcpkg_triplet = "x64-linux"
end
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)

-- Usa path assoluti per le variabili di percorso
sourcedir   = path.getabsolute("src")
outdir      = path.getabsolute("./.build")
sysbuilddir = "%{cfg.system}/%{cfg.buildcfg}"
s2Enginedir = path.getabsolute("../../s2Engine")

-- SOLUTION
workspace "test" 
    location "%{outdir}"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "test"
        
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
        flags { "LinkTimeOptimization" }
    
    filter {}

-- test
project "test"
    kind "ConsoleApp"
    location ("%{outdir}")
    language "C++"
    cppdialect "C++20"
    
    targetdir ( "%{outdir}/bin/%{sysbuilddir}" ) -- i.e. bin/windows/release
    objdir    ( "%{outdir}/tmp/%{sysbuilddir}" )    

    -- list of files
    files {
        "%{sourcedir}/**.h",
        "%{sourcedir}/**.c",
        "%{sourcedir}/**.hpp",
        "%{sourcedir}/**.cpp",
    }
    
    -- additional include directories
    includedirs { 
        "%{sourcedir}/",
        s2Enginedir .. "/include/",
        vcpkg_dir .. "/include",
    }

    libdirs {
        s2Enginedir .. "/bin/%{cfg.system}/%{cfg.buildcfg}/",
        vcpkg_dir .. "/lib"
    }
    
    -- Librerie comuni a tutti i sistemi
    links {
        "s2Engine",
        "imgui",
    }
    
    defines {
    }
       
    filter "system:windows"
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

