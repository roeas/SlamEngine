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

    -- ImGui backends
    files
    {
        path.join(ThirdPartyPath, "imgui/backends/imgui_impl_sdl3.h");
        path.join(ThirdPartyPath, "imgui/backends/imgui_impl_sdl3.cpp");
        path.join(ThirdPartyPath, "imgui/backends/imgui_impl_opengl3_loader.h");
        path.join(ThirdPartyPath, "imgui/backends/imgui_impl_opengl3.h");
        path.join(ThirdPartyPath, "imgui/backends/imgui_impl_opengl3.cpp");
    }

    includedirs
    {
        path.join(ThirdPartyPath, "sdl/include"),
        path.join(ThirdPartyPath, "shaderc/libshaderc/include"),
        path.join(ThirdPartyPath, "assimp/include"),
        path.join(ThirdPartyPath, "assimp/build/include"),
        path.join(ThirdPartyPath, "gli"),
    }

    filter{ "configurations:Debug" }
        libdirs
        {
            path.join(ThirdPartyPath, "sdl/build/Debug"),
            path.join(ThirdPartyPath, "spdlog/build/Debug"),
            path.join(ThirdPartyPath, "shaderc/build/libshaderc/Debug"),
            path.join(ThirdPartyPath, "assimp/build/lib/Debug"),
            path.join(ThirdPartyPath, "assimp/build/contrib/zlib/Debug"),
            path.join(ThirdPartyPath, "tracy/build/Debug"),
        }
        links
        {
            "SDL3", "spdlogd", "shaderc_combined", "assimp-vc143-mtd", "zlibstaticd", "TracyClient",
        }
    filter{ "configurations:Release or configurations:Final" }
        libdirs
        {
            path.join(ThirdPartyPath, "sdl/build/Release"),
            path.join(ThirdPartyPath, "spdlog/build/Release"),
            path.join(ThirdPartyPath, "shaderc/build/libshaderc/Release"),
            path.join(ThirdPartyPath, "assimp/build/lib/Release"),
            path.join(ThirdPartyPath, "assimp/build/contrib/zlib/Release"),
            path.join(ThirdPartyPath, "tracy/build/Release"),
        }
        links
        {
            "SDL3", "spdlog", "shaderc_combined", "assimp-vc143-mt", "zlibstatic", "TracyClient",
        }
    filter{}
