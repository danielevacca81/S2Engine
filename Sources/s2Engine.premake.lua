-- Solution.lua
-- actions: 
--     vs2022
--     gmake2 --os=linux

-- 1. Determina il triplet di vcpkg in base al sistema operativo host
local vcpkg_triplet = "x64-windows-static"
if os.host() == "linux" then
    vcpkg_triplet = "x64-linux"
end
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)


-- SOLUTION
workspace "s2Engine" 
    location "."
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "s2Engine"
    
    -- Usa path assoluti per le variabili di percorso
    sourcedir   = path.getabsolute("s2Engine")
    outdir      = path.getabsolute("./.build")
    deploydir   = path.getabsolute("../s2Engine")
    sysbuilddir  = "%{cfg.system}/%{cfg.buildcfg}"
    
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

-- Main Engine Project
project "s2Engine"
    kind "SharedLib"
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
        "%{sourcedir}/**.inl",
    }
    
    -- additional include directories
    includedirs { 
        vcpkg_dir .. "/include",
        "%{sourcedir}/",
    }

    libdirs {
        vcpkg_dir .. "/lib"
    }
    
    -- Librerie comuni a tutti i sistemi
    links {
        "spdlog",
        "fmt",
        "glfw3",
        "glad"
    }
    
    defines {
        "S2ENGINE_EXPORTS",
    }
    
    postbuildcommands {
        ("{MKDIR}     %{deploydir}/bin/%{sysbuilddir}"),
        ("{MKDIR}     %{deploydir}/include"),
        ("{MKDIR}     %{deploydir}/include/Application"),
        ("{MKDIR}     %{deploydir}/include/Core"),
        ("{MKDIR}     %{deploydir}/include/Geometry"),
        ("{MKDIR}     %{deploydir}/include/Graphics"),
        ("{MKDIR}     %{deploydir}/include/Math"),
        ("{MKDIR}     %{deploydir}/include/RenderCore"),
        ("{MKDIR}     %{deploydir}/include/Renderer"),
        ("{MKDIR}     %{deploydir}/include/Resources"),
        ("{MKDIR}     %{deploydir}/include/Scene"),

        ("{COPYFILE}  %{cfg.buildtarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),
        ("{COPYFILE}  %{cfg.linktarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),
		--("{COPYFILE} %{cfg.targetdir}".."/*.*".." %{deploydir}/bin/%{sysbuilddir}"),
        --
        ("{COPYFILE}  %{sourcedir}/s2Engine_API.h   %{deploydir}/include"),
        ("{COPYFILE}  %{sourcedir}/Application/*.h* %{deploydir}/include/Application"),
        ("{COPYFILE}  %{sourcedir}/Core/*.h*        %{deploydir}/include/Core"),
        ("{COPYFILE}  %{sourcedir}/Geometry/*.h*    %{deploydir}/include/Geometry"),
        ("{COPYFILE}  %{sourcedir}/Graphics/*.h*    %{deploydir}/include/Graphics"),
        ("{COPYFILE}  %{sourcedir}/Math/*.h*        %{deploydir}/include/Math"),
        ("{COPYFILE}  %{sourcedir}/RenderCore/*.h*  %{deploydir}/include/RenderCore"),
        ("{COPYFILE}  %{sourcedir}/Renderer/*.h*    %{deploydir}/include/Renderer"),
        ("{COPYFILE}  %{sourcedir}/Resources/*.h*   %{deploydir}/include/Resources"),
        ("{COPYFILE}  %{sourcedir}/Scene/*.h*       %{deploydir}/include/Scene"),
    }
    
    filter "system:windows"
        links { 
            "opengl32",
            "gdi32",
            "user32",
            "shell32"
        }
        
    filter "system:linux"
        links { 
            "GL",       -- same as opengl32
            "EGL",
            "pthread",  -- Threading
            "dl",       -- Dynamic loading
            "m"         -- Math library base di Linux
        }
        
    filter {} -- Reset filters