print("Generating runtime...")

project("Slam")
    kind("StaticLib")
    language("C++")
    cppdialect("C++20")

    -- Project, binary and intermediate files paths
    location(path.join(RootPath, "Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    files
    {
        path.join(RuntimePath, "**.*"),
    }

    includedirs
    {
        SourcePath,
        RuntimePath,
        ThirdPartyPath,
        path.join(ThirdPartyPath, "spdlog/include"),
        path.join(ThirdPartyPath, "sdl/include"),
    }

    filter{ "configurations:Debug" }
        includedirs
        {
            
        }
        libdirs
        {
            path.join(ThirdPartyPath, "spdlog/build/Debug"),
            path.join(ThirdPartyPath, "sdl/build/Debug"),
        }
        links
        {
            "spdlogd",
            "SDL3",
        }
    filter{ "configurations:Release or configurations:Final" }
        includedirs
        {
            
        }
        libdirs
        {
            path.join(ThirdPartyPath, "spdlog/build/Release"),
            path.join(ThirdPartyPath, "sdl/build/Release"),
        }
        links
        {
            "spdlog",
            "SDL3",
        }
    filter{}
