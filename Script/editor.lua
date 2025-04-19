print("Generating editor...")

project("Editor")
    kind("ConsoleApp")
    language("C++")
    cppdialect("C++20")
    dependson{ "Slam" }

    -- Project, binary and intermediate files paths
    location(path.join(RootPath, "Engine/Project"))
    targetdir(path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))
    objdir(path.join(IntermediatePath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}"))

    files
    {
        path.join(EditorPath, "**.*"),
    }

    includedirs
    {
        EditorPath,
    }

    libdirs
    {
        path.join(BinaryPath, "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}/Slam"),
    }
    links
    {
        "Slam",
    }

    filter{ "configurations:Debug" }
        postbuildcommands 
        {
            "{COPYFILE} %["..path.join(ThirdPartyPath, "sdl/build/Debug/SDL3.dll").."] %[%{!cfg.targetdir}]",
        }
    filter{ "configurations:Release or configurations:Final" }
        postbuildcommands 
        {
            "{COPYFILE} %["..path.join(ThirdPartyPath, "sdl/build/Release/SDL3.dll").."] %[%{!cfg.targetdir}]",
        }
    filter{}
