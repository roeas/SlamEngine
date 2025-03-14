print("Generating utility...")

project("Build")
    kind("Utility")

    -- Project, binary and intermediate file paths
    location(path.join(RootPath, "Project"))
    targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))

    -- Files
    files
    {
        path.join(RootPath, "Setup.bat"),
        -- path.join(RootPath, "SetupThirdParty.bat"),
        path.join(RootPath, "Script/**.lua"),
    }

    postbuildcommands
    {
        path.join(RootPath, "Setup.bat"),
    }
