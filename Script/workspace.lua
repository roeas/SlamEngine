workspace("SlamEngine")
    location(RootPath)

    architecture("x64")
    configurations{ "Debug", "Release", "Final" }

    -- Debug mode, no optimization
    filter{ "configurations:Debug" }
        defines{ "SL_DEBUG" }
        symbols("On")
        optimize("Off")
        runtime("Debug") -- /MTd

    -- Release mode
    filter{ "configurations:Release" }
        defines{ "SL_RELEASE" }
        symbols("On")
        optimize("On")
        runtime("Release") -- /MT

    -- Final maode, full optimization
    filter{ "configurations:Final" }
        defines{ "SL_FINAL" }
        symbols("Off")
        optimize("Speed")
        runtime("Release") -- /MT

    filter{ "system:Windows" }
        systemversion("latest")
        defines{ "SL_WINDOWS" }

    filter{}
    exceptionhandling("Off")
    fatalwarnings{ "All" }
    justmycode("Off")
    rtti("Off")
    staticruntime("on")

    defines
    {
        "SL_ROOT_PATH=\""..RootPath.."\"",
        "SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
    }

    flags
    {
        -- Enable Visual Studio to use multiple compiler processes when building.
        "MultiProcessorCompile",
    }

    startproject("Editor")
