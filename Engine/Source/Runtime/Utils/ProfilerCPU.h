#pragma once

#include "Utils/Profiler.h"

#if defined(TRACY_ENABLE)
    #define SL_PROFILE_FRAME FrameMark

    #define SL_PROFILE ZoneScoped
    #define SL_PROFILE_NAME(x) ZoneScopedN(x)
    #define SL_PROFILE_COLOR(x) ZoneScopedC(x)
    #define SL_PROFILE_NAME_COLR(n, c) ZoneScopedN(n, c)
    #define SL_PROFILE_ADD_TEXT(t, s) ZoneText(t, s)
    #define SL_PROFILE_ADD_VALUE(x) ZoneValue(x)

    #define SL_PROFILE_PLOT(n, v) TracyPlot(n, v)
#else
    #define SL_PROFILE_FRAME
    #define SL_PROFILE
    #define SL_PROFILE_NAME(x)
    #define SL_PROFILE_COLOR(x)
    #define SL_PROFILE_NAME_COLR(n, c)
    #define SL_PROFILE_ADD_TEXT(t, s)
    #define SL_PROFILE_ADD_VALUE(x)
    #define SL_PROFILE_PLOT(n, v)
#endif
