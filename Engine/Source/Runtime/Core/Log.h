#pragma once

#include "Core/Core.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <vector>

namespace sl
{

enum class LogLevel : uint8_t
{
    Trace = 1 << 0,
    Debug = 1 << 1,
    Info  = 1 << 2,
    Warn  = 1 << 3,
    Error = 1 << 4,
    Fatal = 1 << 5,
    Off   = 1 << 6,
};

struct LogInfo
{
    LogLevel m_level;
    std::string m_content;
};

class Log final
{
public:
    Log() = delete;
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;
    Log(Log &&) = delete;
    Log &operator=(Log &&) = delete;
    ~Log() = delete;

    static void Init();
    static spdlog::logger *GetEngineLogger() { return m_pEngineLogger.get(); }
    static std::vector<LogInfo> &GetLogInfos() { return m_logInfos; }

private:
    inline static std::unique_ptr<spdlog::logger> m_pEngineLogger;
    inline static std::vector<LogInfo> m_logInfos;
};

} // namespace sl

#define SL_LOG_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define SL_LOG_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
#define SL_LOG_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
#define SL_LOG_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define SL_LOG_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
#define SL_LOG_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)

#if !defined(SL_FINAL)
    #define SL_ASSERT(x, ...) { if(!(x)) { __VA_OPT__(SL_LOG_FATAL(__VA_ARGS__);) SL_BREAKPOINT(); } }
#else
    #define SL_ASSERT(...)
#endif

// GLM vector std::formatter specialize
#define SL_FMT_PLACEHOLDER_1
#define SL_FMT_PLACEHOLDER_2 "{}" ", " "{}"
#define SL_FMT_PLACEHOLDER_3 SL_FMT_PLACEHOLDER_2 ", " "{}"
#define SL_FMT_PLACEHOLDER_4 SL_FMT_PLACEHOLDER_3 ", " "{}"
#define SL_FMT_PLACEHOLDER(N) SL_FMT_PLACEHOLDER_##N

#define SL_COMPONENT_VEC_1 vec.x
#define SL_COMPONENT_VEC_2 SL_COMPONENT_VEC_1, vec.y
#define SL_COMPONENT_VEC_3 SL_COMPONENT_VEC_2, vec.z
#define SL_COMPONENT_VEC_4 SL_COMPONENT_VEC_3, vec.w
#define SL_COMPONENT_VEC(N) SL_COMPONENT_VEC_##N

#define SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(PREFIX, N) \
template <> \
struct std::formatter<glm::PREFIX##vec##N> : std::formatter<std::string> { \
    auto format(const glm::PREFIX##vec##N& vec, std::format_context& ctx) const { \
        return std::formatter<std::string>::format( \
            std::format(#PREFIX "vec" #N "(" SL_FMT_PLACEHOLDER(N) ")", SL_COMPONENT_VEC(N)), ctx); \
    } \
};

SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(, 2);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(, 3);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(, 4);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(i, 2);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(i, 3);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(i, 4);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(u, 2);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(u, 3);
SL_GLM_VECTOR_FORMATTER_SPECIALIZATION(u, 4);
