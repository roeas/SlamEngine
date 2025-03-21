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

    files
    {
        path.join(EditorPath, "**.*"),
    }

    includedirs
    {
        EditorPath,
        RuntimePath,
        path.join(ThirdPartyPath, "spdlog/include"),
    }

    libdirs
    {
        path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Slam"),
    }
    links
    {
        "Slam",
    }
