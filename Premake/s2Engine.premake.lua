-- Solution.lua
-- actions: 
--     vs2022
--     gmake2 --os=linux

-- SOLUTION
workspace "s2Engine" 
	location "../Build"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "s2Engine"
	
    -- solution variables
	sourcedir  = "../Sources/s2Engine"
	extlibsdir = "../ExtLibs"
	outdir     = "../Build"
	deploydir  = "../s2Engine"
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
	location "../Build"	
	language "C++"
	cppdialect "C++20"
	
	
	targetdir ( "%{outdir}/bin/%{sysbuilddir}" ) -- i.e. bin/windows/release
	objdir    ( "%{outdir}/tmp/%{sysbuilddir}" )	
		
    -- IDE Grouping @todo
	--vpaths { ["Core"] = {"Core/**.h","Core/**.hpp","Core/**.cpp"} }

	-- list of files
	files 
	{
		"%{sourcedir}/**.h",
		"%{sourcedir}/**.c",
		"%{sourcedir}/**.hpp",
		"%{sourcedir}/**.cpp",
	}
	
	-- additional include directories
	includedirs
	{ 
		"%{sourcedir}/",
		"%{extlibsdir}/glfwpp/include",
	}
	
    links {
        "opengl32"
    }
	
    defines {
        "S2ENGINE_EXPORTS",
    }
	
	postbuildcommands {
	    ("{MKDIR} %{deploydir}/bin/%{sysbuilddir}"),
		("{MKDIR} %{deploydir}/include"),
		("{COPYFILE} %{cfg.buildtarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),
		("{COPYFILE} %{cfg.linktarget.relpath} %{deploydir}/bin/%{sysbuilddir}"),
		--
		("{COPYFILE} %{sourcedir}/s2Engine_API.h %{deploydir}/include"),
		("{COPYDIR} %{sourcedir}/Application/*.h*    %{deploydir}/include/Application"),
		("{COPYDIR} %{sourcedir}/Core/*.h*           %{deploydir}/include/Core"),
		("{COPYDIR} %{sourcedir}/Geometry/*.h*       %{deploydir}/include/Geometry"),
		("{COPYDIR} %{sourcedir}/Graphics/*.h*       %{deploydir}/include/Graphics"),
		("{COPYDIR} %{sourcedir}/Math/*.h*           %{deploydir}/include/Math"),
		("{COPYDIR} %{sourcedir}/RenderCore/*.h*     %{deploydir}/include/RenderCore"),
		("{COPYDIR} %{sourcedir}/Renderer/*.h*       %{deploydir}/include/Renderer"),
		("{COPYDIR} %{sourcedir}/Resources/*.h*      %{deploydir}/include/Resources"),
		("{COPYDIR} %{sourcedir}/Scene/*.h*          %{deploydir}/include/Scene"),
		("{COPYDIR} %{sourcedir}/UI/*.h*             %{deploydir}/include/UI"),
	}
    
	
	-- -- specifc for windows
	-- filter "system:windows"
	  -- --staticruntime "On"
	  -- systemversion "latest"
	  -- defines { "S2ENGINE_EXPORTS" }
	  
	-- filter "configurations:Debug"
	  -- --defines { "_DEBUG" }
	  -- symbols "On"

	-- filter "configurations:Release"
	  -- defines { "NDEBUG" }
	  -- optimize "On"
	  -- symbols "On"	  
	
	-- filter{} -- close filters


	-- postbuildcommands 
	-- {
		-- ("{MKDIR} %{deploydir}/bin"),
		-- ("{MKDIR} %{deploydir}/include"),
		-- ("{COPYFILE} %{cfg.buildtarget.relpath} %{deploydir}/%{cfg.buildtarget.directory}"), -- copy .dll
		-- ("{COPYFILE} %{cfg.buildtarget.relpath} %{deploydir}/%{cfg.buildtarget.directory}"), -- copy .dll
		-- ("{COPYFILE} %{cfg.longname} %{deploydir}/%{cfg.linktarget.relpath}"),   -- copy .lib
	-- }
	

	-- filter { "system:windows","configurations:Release"}
		-- --buildoptions "/MD"
		-- copySDK( "x64", "Release", "dll", "%{pkgBinDir}")
		-- copySDK( "x64", "Release", "lib", "%{pkgBinDir}")
		
		
	-- filter { "system:windows","configurations:Debug"}
		-- --buildoptions "/MDd"
		-- copySDK( "x64", "Debug", "dll", "%{pkgBinDir}")
		-- copySDK( "x64", "Debug", "lib", "%{pkgBinDir}")
	
	-- filter { "system:linux","configurations:Release"}		
		-- copySDK( "Linux64", "Release", "so", "%{pkgBinDir}")
		
	-- filter { "system:linux","configurations:Debug"}		
		-- copySDK( "Linux64", "Debug", "so", "%{pkgBinDir}")		
   
   --filter{} -- close filters   
