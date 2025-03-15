print("Generating runtime...")

project("Slam")
    kind("StaticLib")
    language("C++")
    cppdialect("C++20")

    -- Project, binary and intermediate file paths
    location(path.join(RootPath, "Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    files
    {
        path.join(RuntimePath, "**.*"),
    }

    includedirs
    {
        RuntimePath,
    }

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
