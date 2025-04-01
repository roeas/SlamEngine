#pragma once

#include "Core/Core.h"

#include <spdlog/spdlog.h>

#include <cstdint>
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
