#pragma once

#include "Utils/Profiler.h"

#if defined(TRACY_ENABLE)
    /*
     * At the moment we have to include #gl.h in editor,
     * maybe we can improve this when the Render Pass concept is refined.
     */
    #include <glad/gl.h>
    #include <tracy/TracyOpenGL.hpp>
    
    #define SL_PROFILE_GPU_CONTEXT TracyGpuContext
    #define SL_PROFILE_GPU_COLLECT TracyGpuCollect
    
    #define SL_PROFILE_GPU(x) TracyGpuZone(x)
#else
    #define SL_PROFILE_GPU_CONTEXT
    #define SL_PROFILE_GPU_COLLECT
    #define SL_PROFILE_GPU(x)
#endif
