#include "Log.h"

#include "Core/Path.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

namespace sl
{

namespace
{

constexpr std::array<LogLevel, 6> SpdLevelToSLLevel =
{
    LogLevel::Trace,
    LogLevel::Debug,
    LogLevel::Info,
    LogLevel::Warn,
    LogLevel::Error,
    LogLevel::Fatal,
};

} // namespace

void Log::Init()
{
    // Output to console.
    auto pConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    pConsoleSink->set_pattern("%^[%T] %n: %v%$");

    // Output to file.
    auto pFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Path::FromeRoot("Engine/Log/Slam.log"), true);
    pFileSink->set_pattern("[%T] [%l] %n: %v");

    // Output to buffer and show on ui.
    auto pCallbackSink = std::make_shared<spdlog::sinks::callback_sink_mt>([](const spdlog::details::log_msg &msg)
    {
        // TODO: Add time information to log buffer.
        m_logInfos.emplace_back
        (
            SpdLevelToSLLevel[msg.level],
            std::string{ msg.payload.data(), msg.payload.size() }
        );
    });

    std::vector<spdlog::sink_ptr> sinks{ pConsoleSink, pFileSink, pCallbackSink };
    m_pEngineLogger = std::make_unique<spdlog::logger>("Engine", sinks.begin(), sinks.end());
    m_pEngineLogger->set_level(spdlog::level::trace);
}

} // namespace sl
