-- test.premake.lua

local vcpkg_triplet = (os.host() == "windows") and "x64-windows-static-md" or "x64-linux"
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)


-- Usa path assoluti per le variabili di percorso
sourcedir   = path.getabsolute("src")
outdir      = path.getabsolute(".build")
sysbuilddir = "%{cfg.system}/%{cfg.buildcfg}"
s2Enginedir = path.getabsolute("../../s2Engine")

-- SOLUTION
workspace "test" 
    location "%{outdir}"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "test"
        
    flags { "MultiProcessorCompile"}
    cppdialect "C++20"
    
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
    
    

-- test
project "test"
    kind "ConsoleApp"
    location "%{outdir}"
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
        vcpkg_dir .. "/include",
        s2Enginedir .. "/include/",
    }

    libdirs {
        s2Enginedir .. "/bin/%{cfg.system}/%{cfg.buildcfg}/",
    }

    links {
        "s2Engine",
    }

       
    defines {}

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"
        optimize "Off"
    filter { "configurations:Release" }
        defines { "NDEBUG" }
        symbols "On"
        optimize "Speed"
        --flags { "LinkTimeOptimization" }
    filter {}

    filter "system:linux"
        libdirs {
            vcpkg_dir .. "/lib"
        }
        links {
            -- "spdlog",
            -- "fmt",
            -- "glfw3",
            -- "glad",
            "imgui",
        }

    filter { "system:windows", "configurations:Debug" }
        staticruntime "Off"
        runtime "Debug"
        libdirs {
            vcpkg_dir .. "/debug/lib",
        }
        links{
            "imguid"
        }

    filter { "system:windows", "configurations:Release" }
        staticruntime "Off"
        runtime "Release"
        defines { }
        libdirs {
            vcpkg_dir .. "/lib"
        }
        links{
            "imgui"
        }        
    filter {}





    filter "system:windows"
        postbuildcommands {
            ("{COPYFILE} %{s2Enginedir}/bin/%{cfg.system}/%{cfg.buildcfg}/s2Engine.dll %{cfg.targetdir}"),
        }    
        
    filter "system:linux"
        postbuildcommands {
            ("{COPYFILE} %{s2Enginedir}/bin/%{cfg.system}/%{cfg.buildcfg}/libs2Engine.so %{cfg.targetdir}"),
        }
        linkoptions { "-Wl,-rpath='$$ORIGIN'" }
        
    filter {} -- Reset filters

