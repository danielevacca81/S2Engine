-- Solution.lua
-- actions: 
--     vs2022
--     gmake2 --os=linux
--
function addSDKlibs()
	filter { "system:windows","configurations:Release"}
		libdirs{ "../SDK/SDK/bin/x64/Release" }
		
	filter { "system:windows","configurations:Debug"}
		libdirs{ "../SDK/SDK/bin/x64/Debug" }
		
	filter { "system:linux","configurations:Debug"}
		libdirs{"../SDK/SDK/bin/Linux64/Debug" }
		
	filter { "system:linux","configurations:Release"}
		libdirs{"../SDK/SDK/bin/Linux64/Release" }
		
	filter{}
	-- linked libraries
	links { "Core", "RadarConfiguration", "Network", "RadarInterface", "ProcessingKernel", "SerialInterface" }	
end

function copySDK( sys, src, ext, dest)
	-- copy SDK libraries	
	postbuildcommands
	{
		("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*Core."..ext.." "..dest),
		("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*Network."..ext.." "..dest),
		("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*RadarConfiguration."..ext.." "..dest),
		("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*RadarInterface."..ext.." "..dest),
		("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*ProcessingKernel."..ext.." "..dest),
        ("{COPYFILE} %{wks.location}/../SDK/SDK/bin/"..sys.."/"..src.."/*SerialInterface."..ext.." "..dest),
	}
end



workspace "s2Engine" -- aka solution
	location "../Build"
	architecture "x64"
	configurations { "Debug", "Release" }
	--startproject "Core"
	

-- PROJECT s2Engine
project "s2Engine"
	location "../Build"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	
	sourcedir = "../Sources/s2Engine"
	extlibsdir = "../ExtLibs"
	
	--defines {"GLEW_STATIC"}
	 
    -- IDE Grouping @todo
	--vpaths { ["Core"] = {"Core/**.h","Core/**.hpp","Core/**.cpp"} }

	-- list of files
	files 
	{
		"%{sourcedir}/**.h",
		"%{sourcedir}/**.c",
		"%{sourcedir}/**.hpp",
		"%{sourcedir}/**.cpp",
		--"%{extlibsdir}/glew-2.3.1/src/glew.c",
	}
	
	-- additional include directories
	includedirs
	{ 
		"%{sourcedir}/",
		-- "%{extlibsdir}/glm-1.0.1",
		"%{extlibsdir}/glfwpp/include",
	}
	
	libdirs{}
	
	targetdir ( "../Build/bin/%{cfg.system}/%{cfg.buildcfg}" ) -- i.e. bin/windows/release
	objdir    ( "../Build/tmp/%{cfg.system}/%{cfg.buildcfg}" )
	
	-- specifc for windows
	filter "system:windows"
	  --staticruntime "On"
	  systemversion "latest"
	  --links { "glew32.lib" }
	  defines { "S2ENGINE_EXPORTS" }
	  
	filter "configurations:Debug"
	  --defines { "_DEBUG" }
	  symbols "On"

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  symbols "On"	  
	
	filter{} -- close filters


	-- postbuildcommands 
	-- {
		-- ("{MKDIR} %{pkgBinDir}"),
		-- ("{MKDIR} %{pkgIncDir}"),
		-- ("{COPYFILE} %{cfg.buildtarget.relpath} %{pkgBinDir}"),
		-- ("{COPYFILE} %{cfg.linktarget.relpath} %{pkgBinDir}"),
		-- ("{COPYFILE} %{prj.location}/src/DigX_API.h %{pkgIncDir}"),		
		-- ("{COPYFILE} %{prj.location}/src/DigXInterface.h %{pkgIncDir}"),
		-- ("{COPYFILE} %{prj.location}/src/DigXTypes.h %{pkgIncDir}"),
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
   
   filter{} -- close filters   
