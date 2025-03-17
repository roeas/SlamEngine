workspace("SlamEngine")
    location(RootPath)

    systemversion("latest")
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

    filter{}
    exceptionhandling("Off") -- No exception
    fatalwarnings{ "All" } -- Treat all compiler warnings as errors
    rtti("Off") -- No RTTI
    staticruntime("On") -- Use /MT instead of /MD
    usestandardpreprocessor("On") -- Enable __VA_OPT__

    defines
    {
        "SL_ROOT_PATH=\""..RootPath.."\"",
        "SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
        "SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT",
    }

    includedirs
    {
        SourcePath,
        ThirdPartyPath,
        path.join(ThirdPartyPath, "spdlog/include"),
    }

    flags
    {
        -- Enable Visual Studio to use multiple compiler processes when building.
        "MultiProcessorCompile",
    }

    startproject("Editor")
