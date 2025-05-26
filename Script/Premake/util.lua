print("Generating utility...")

project("Build")
    kind("Utility")

    -- Project, binary and intermediate files paths
    location(path.join(RootPath, "Engine/Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    files
    {
        path.join(RootPath, "Script/*.bat"),
        path.join(RootPath, "Script/Premake/*.lua"),
    }

    postbuildcommands
    {
        path.join(RootPath, "Script/Premake.bat"),
    }

    flags
    {
        "ExcludeFromBuild",
    }
