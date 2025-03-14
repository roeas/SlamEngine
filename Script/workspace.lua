workspace("SlamEngine")
    location(RootPath)

    architecture("x64")
    configurations{ "Debug", "Release", "Final" }
    staticruntime "on" -- Use static C runtime library

    -- Debug mode, no optimization
    filter { "configurations:Debug" }
        defines { "SL_DEBUG" }
        symbols("On")
        optimize("Off")
        runtime("Debug") -- /MTd

    -- Release mode
    filter { "configurations:Release" }
        defines { "SL_RELEASE" }
        symbols("On")
        optimize("On")
        runtime("Debug") -- /MTd

    -- Final maode, full optimization
    filter { "configurations:Final" }
        defines { "SL_FINAL" }
        symbols("Off")
        optimize("Full")
        runtime("Release") -- /MT

    filter { "system:Windows" }
        defines { "SL_WINDOWS" }
        systemversion("latest")

    filter {}
    rtti("Off")
    exceptionhandling("Off")
    justmycode("Off")
	fatalwarnings { "All" }

    flags
    {
        -- Enable Visual Studio to use multiple compiler processes when building.
        "MultiProcessorCompile",
    }

    startproject("Editor")
