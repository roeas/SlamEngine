workspace("SlamEngine")
    location(RootPath)

    systemversion("latest")
    architecture("x64")
    configurations{ "Debug", "Release", "Final" }

    -- Debug mode, no optimization
    filter{ "configurations:Debug" }
        defines{ "SL_DEBUG", "DEBUG" }
        symbols("On")
        optimize("Debug")
        runtime("Debug")
        linktimeoptimization("Off")

    -- Release mode
    filter{ "configurations:Release" }
        defines{ "SL_RELEASE", "NDEBUG" }
        symbols("On")
        optimize("On")
        runtime("Release")
        linktimeoptimization("Off")

    -- Final maode, full optimization
    filter{ "configurations:Final" }
        defines{ "SL_FINAL", "NDEBUG" }
        symbols("On")
        optimize("Speed")
        runtime("Release")
        linktimeoptimization("On")

    filter{}
    exceptionhandling("Off") -- No exception
    fatalwarnings{ "All" } -- Treat all compiler warnings as errors
    rtti("Off") -- No RTTI
    staticruntime("Off") -- Use /MD and /MDd
    usestandardpreprocessor("On") -- Enable C++20 new preprocessor

    defines
    {
        "SL_ROOT_PATH=\""..RootPath.."\"",
        "SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
        "SPDLOG_COMPILED_LIB", "SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT"
    }

    includedirs
    {
        RuntimePath,
        ThirdPartyPath,
        path.join(ThirdPartyPath, "spdlog/include"),
        path.join(ThirdPartyPath, "imgui"),
        path.join(ThirdPartyPath, "entt/src"),
    }

    flags
    {
        -- Enable Visual Studio to use multiple compiler processes when building
        "MultiProcessorCompile",
    }

    startproject("Editor")
