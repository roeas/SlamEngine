#pragma once

#include "Utils/Profiler.h"

#define SL_PROFILE_FRAME FrameMark

#define SL_PROFILE ZoneScoped
#define SL_PROFILE_NAME(x) ZoneScopedN(x)
#define SL_PROFILE_COLOR(x) ZoneScopedC(x)
#define SL_PROFILE_NAME_COLR(n, c) ZoneScopedN(n, c)
#define SL_PROFILE_ADD_VALUE(x) ZoneValue(x)

#define SL_PROFILE_PLOT(n, v) TracyPlot(n, v)
