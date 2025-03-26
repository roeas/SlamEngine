#include "Log.h"

#include "Core/Path.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sl
{

namespace
{

constexpr std::array<LogLevel, 7> SPDLevelToSLLevel =
{
    LogLevel::Trace,
    LogLevel::Debug,
    LogLevel::Info,
    LogLevel::Warn,
    LogLevel::Error,
    LogLevel::Fatal,
    LogLevel::Off,
};

} // namespace

void Log::Init()
{
    // Output to console
    auto pConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    pConsoleSink->set_pattern("%^[%T] [%n] %v%$");

    // Output to file
    auto pFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Path::FromeRoot("Engine/Log/Slam.log"), true);
    pFileSink->set_pattern("[%T] [%n] [%l] %v");

    // Output to buffer and show on ui
    auto pCallbackSink = std::make_shared<spdlog::sinks::callback_sink_mt>([](const spdlog::details::log_msg &msg)
    {
        // TODO: Ring buffer
        // TODO: Add time information to #m_logInfos
        m_logInfos.emplace_back
        (
            SPDLevelToSLLevel[msg.level],
            std::string{ msg.payload.data(), msg.payload.size() }
        );
    });

    std::vector<spdlog::sink_ptr> sinks{ pConsoleSink, pFileSink, pCallbackSink };
    m_pEngineLogger = std::make_unique<spdlog::logger>("Engine", sinks.begin(), sinks.end());
    m_pEngineLogger->set_level(spdlog::level::trace);

    SL_LOG_INFO("Logger initialized");
}

} // namespace sl
