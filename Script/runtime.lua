print("Generating runtime...")

project("Slam")
    kind("StaticLib")
    language("C++")
    cppdialect("C++20")

    -- Project, binary and intermediate file paths
    location(path.join(RootPath, "Project"))
    targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))

    defines
    {
        "SL_ROOT_PATH=\""..RootPath.."\"",
        "SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
    }

    -- Include paths
    includedirs
    {
        SourcePath,
        RuntimePath,
        ThirdPartyPath,
    }

    -- Files
    files
    {
        path.join(RuntimePath, "**.*"),
    }

    -- Thirdparty libs
    filter { "configurations:Debug" }
        includedirs
        {
            
        }
        libdirs
        {
            
        }
        links
        {
            
        }
    filter { "configurations:Release or configurations:Final" }
        includedirs
        {
            
        }
        libdirs
        {
            
        }
        links
        {
            
        }
    filter {}
