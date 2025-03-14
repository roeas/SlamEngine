print("Generating editor...")

project("Editor")
    kind("ConsoleApp")
    language("C++")
    cppdialect("C++20")
    dependson { "Slam" }

    -- Project, binary and intermediate file paths
    location(path.join(RootPath, "Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    -- Include paths
    includedirs
    {
        SourcePath,
		EditorPath,
        RuntimePath,
        ThirdPartyPath,
    }

    -- Files
    files
    {
        path.join(EditorPath, "**.*"),
    }

    -- Thirdparty libs
    filter { "configurations:Debug" }
        libdirs
        {
            -- path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Slam"),
        }
        links
        {
            -- "Slam",
        }
    filter { "configurations:Release or configurations:Final" }
        libdirs
        {
            -- path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Slam"),
        }
        links
        {
            -- "Slam",
        }
    filter {}
