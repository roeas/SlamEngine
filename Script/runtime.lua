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
        path.join(ThirdPartyPath, "glad/**.*");
        path.join(ThirdPartyPath, "imgui/*.h");
        path.join(ThirdPartyPath, "imgui/*.cpp");
    }

    includedirs
    {
        path.join(ThirdPartyPath, "sdl/include"),
        path.join(ThirdPartyPath, "glad/include"),
    }

    filter{ "configurations:Debug" }
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
