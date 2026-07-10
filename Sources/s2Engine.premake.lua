-- Solution.lua
-- actions:
--     vs2022
--     gmake2 --os=linux

local vcpkg_triplet = (os.host() == "windows") and "x64-windows-static-md" or "x64-linux"
local vcpkg_dir = path.getabsolute("./vcpkg_installed/" .. vcpkg_triplet)

workspace "s2Engine"
    location ".build"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "s2Engine"

    sourcedir   = path.getabsolute("s2Engine")
    outdir      = path.getabsolute(".build")
    deploydir   = path.getabsolute("../s2Engine")
    sysbuilddir = "%{cfg.system}/%{cfg.buildcfg}"

    flags { "MultiProcessorCompile" }
    cppdialect "C++20"

    filter "system:windows"
        systemversion "latest"
    filter {}

project "s2Engine"
    kind "SharedLib"
    location "%{outdir}"
    language "C++"
    cppdialect "C++20"

    targetdir("%{outdir}/bin/%{sysbuilddir}")
    objdir("%{outdir}/tmp/%{sysbuilddir}")

    files {
        "%{sourcedir}/**.h",
        "%{sourcedir}/**.c",
        "%{sourcedir}/**.hpp",
        "%{sourcedir}/**.cpp",
        "%{sourcedir}/**.inl",
    }

    includedirs {
        "%{sourcedir}/",
        vcpkg_dir .. "/include",
    }

    defines { "S2ENGINE_EXPORTS", "SPDLOG_COMPILED_LIB" }
    
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
            "spdlog",
            "fmt",
            "glfw3",
            "glad",
            "imgui",
        }

    filter { "system:windows", "configurations:Debug" }
        staticruntime "Off"
        runtime "Debug"        
        defines { "GLFW_STATIC" }

        libdirs {
            vcpkg_dir .. "/debug/lib",
        }
        links {
            "spdlogd",
            "fmtd",
            "glfw3",
            "glad",
            "imguid",
            "opengl32",
            "gdi32",
            "user32",
            "shell32",
        }

    filter { "system:windows", "configurations:Release" }
        staticruntime "Off"
        runtime "Release"
        defines { "GLFW_STATIC" }

        libdirs {
            vcpkg_dir .. "/lib"
        }
        links {
            "spdlog",
            "fmt",
            "glfw3",
            "glad",
            "imgui",
            "opengl32",
            "gdi32",
            "user32",
            "shell32",
        }
    filter {}

    postbuildcommands {
        ("{MKDIR}     %{deploydir}/bin/%{sysbuilddir}"),
        ("{MKDIR}     %{deploydir}/include"),
        ("{MKDIR}     %{deploydir}/include/Application"),
        ("{MKDIR}     %{deploydir}/include/Core"),
        ("{MKDIR}     %{deploydir}/include/ECS"),
        ("{MKDIR}     %{deploydir}/include/Geometry"),
        ("{MKDIR}     %{deploydir}/include/Graphics"),
        ("{MKDIR}     %{deploydir}/include/Math"),
        ("{MKDIR}     %{deploydir}/include/RenderCore"),
        ("{MKDIR}     %{deploydir}/include/Renderer"),
        ("{MKDIR}     %{deploydir}/include/Resources"),
        ("{MKDIR}     %{deploydir}/include/Scene"),
        ("{MKDIR}     %{deploydir}/include/Scene/Systems"),
        ("{MKDIR}     %{deploydir}/include/Scene/Controls"),

        ("{COPYFILE}  %{cfg.buildtarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),
        ("{COPYFILE}  %{cfg.linktarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),

        ("{COPYFILE}  %{sourcedir}/s2Engine_API.h      %{deploydir}/include"),
        ("{COPYFILE}  %{sourcedir}/Application/*.h*    %{deploydir}/include/Application"),
        ("{COPYFILE}  %{sourcedir}/Core/*.h*           %{deploydir}/include/Core"),
        ("{COPYFILE}  %{sourcedir}/ECS/*.h*            %{deploydir}/include/ECS"),
        ("{COPYFILE}  %{sourcedir}/Geometry/*.h*       %{deploydir}/include/Geometry"),
        ("{COPYFILE}  %{sourcedir}/Graphics/*.h*       %{deploydir}/include/Graphics"),
        ("{COPYFILE}  %{sourcedir}/Math/*.h*           %{deploydir}/include/Math"),
        ("{COPYFILE}  %{sourcedir}/RenderCore/*.h*     %{deploydir}/include/RenderCore"),
        ("{COPYFILE}  %{sourcedir}/Renderer/*.h*       %{deploydir}/include/Renderer"),
        ("{COPYFILE}  %{sourcedir}/Resources/*.h*      %{deploydir}/include/Resources"),
        ("{COPYFILE}  %{sourcedir}/Scene/*.h*          %{deploydir}/include/Scene"),
        ("{COPYFILE}  %{sourcedir}/Scene/Systems/*.h*  %{deploydir}/include/Scene/Systems"),
        ("{COPYFILE}  %{sourcedir}/Scene/Controls/*.h* %{deploydir}/include/Scene/Controls"),
    }