print("Generating runtime...")

project("Slam")
    kind("StaticLib")
    language("C++")
    cppdialect("C++20")

    -- Project, binary and intermediate files paths
    location(path.join(RootPath, "Engine/Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    files
    {
        path.join(RuntimePath, "**.*"),
        path.join(ThirdPartyPath, "glad/**.h");
        path.join(ThirdPartyPath, "glad/**.c");
        path.join(ThirdPartyPath, "imgui/*.h");
        path.join(ThirdPartyPath, "imgui/*.cpp");
        path.join(ThirdPartyPath, "implot/*.h");
        path.join(ThirdPartyPath, "implot/*.cpp");
        path.join(ThirdPartyPath, "imguizmo/*.h");
        path.join(ThirdPartyPath, "imguizmo/*.cpp");
    }

    includedirs
    {
        path.join(ThirdPartyPath, "sdl/include"),
        path.join(ThirdPartyPath, "glad/include"),
    }

    filter{ "configurations:Debug" }
        libdirs
        {
            path.join(ThirdPartyPath, "sdl/build/Debug"),
            path.join(ThirdPartyPath, "spdlog/build/Debug"),
        }
        links
        {
            "SDL3", "spdlogd"
        }
    filter{ "configurations:Release or configurations:Final" }
        libdirs
        {
            path.join(ThirdPartyPath, "sdl/build/Release"),
            path.join(ThirdPartyPath, "spdlog/build/Release"),
        }
        links
        {
            "SDL3", "spdlog"
        }
    filter{}
