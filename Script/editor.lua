print("Generating editor...")

project("Editor")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++20")
	dependson { "Slam" }
	
	-- Project, binary and intermediate files path.
	location(IntermediatePath)
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Set definitions.
	defines
	{
		"SPDLOG_USE_STD_FORMAT",
		"SPDLOG_NO_EXCEPTIONS",
	}
	
	filter "configurations:Debug"
		defines { "SL_DEBUG" }
	filter "configurations:Release"
		defines { "SL_RELEASE" }
	filter "configurations:Final"
		defines { "SL_FINAL" }
	filter "system:Windows"
		defines { "SL_WINDOWS" }
	filter {}
	
	-- Set include paths.
	includedirs
	{
		EnginePath,
		EditorPath,
		ThirdPartyPath,
		
		path.join(ThirdPartyPath, "spdlog/include"),
	}
	
	-- Set files.
	files
	{
		path.join(EditorPath, "**.*"),
		path.join(ThirdPartyPath, "spdlog/include/**.*"),
	}
	
	-- Link to engine.
	libdirs
	{
		path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
	}
	links
	{
		"Slam",
	}
	
	-- Link to thirdparty libs.
	
	-- Use /MT and /MTd.
	staticruntime "on"
	filter "configurations:Debug"
		runtime("Debug") -- /MTd
	filter "configurations:Release"
		runtime("Release") -- /MT
	filter "configurations:Final"
		runtime("Release") -- /MT
	filter {}
	
	-- Some optimize.
	justmycode("Off")
	editAndContinue("Off")
	exceptionhandling("Off")
	rtti("Off")
	
	-- Strict.
	warnings("Default")
	externalwarnings("Off")
	
	flags
	{
		-- Compiler uses multiple thread.
		"MultiProcessorCompile",
		-- treat warnings as errors
		"FatalWarnings",
	}
	
print("")