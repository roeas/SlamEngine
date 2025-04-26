#include "OutputLog.h"

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <iconfontcppheaders/IconsMaterialSymbols.h>
#include <imgui/imgui.h>

void OutputLog::OnAttach()
{

}

void OutputLog::OnDetach()
{

}

void OutputLog::BeginFrame()
{

}

void OutputLog::OnUpdate(float deltaTime)
{
    SL_PROFILE;

    // 0011 1111, which means not filtering anything
    constexpr uint8_t FullLevelFilter = 0x3f;
    static uint8_t s_levelFilter = FullLevelFilter;
    static ImGuiTextFilter s_textFilter;

    constexpr ImVec4 TraceColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    constexpr ImVec4 DebugColor{ 0.1f, 0.5f, 1.0f, 1.0f };
    constexpr ImVec4 InfoColor { 0.1f, 0.7f, 0.1f, 1.0f };
    constexpr ImVec4 WarnColor { 0.9f, 0.8f, 0.2f, 1.0f };
    constexpr ImVec4 ErrorColor{ 1.0f, 0.2f, 0.2f, 1.0f };
    constexpr ImVec4 FatalColor{ 0.9f, 0.2f, 0.9f, 1.0f };

    auto LogLevelToColor = [](sl::LogLevel level)
    {
        switch (level)
        {
            case sl::LogLevel::Trace:
            {
                return TraceColor;
            }
            case sl::LogLevel::Debug:
            {
                return DebugColor;
            }
            case sl::LogLevel::Info:
            {
                return InfoColor;
            }
            case sl::LogLevel::Warn:
            {
                return WarnColor;
            }
            case sl::LogLevel::Error:
            {
                return ErrorColor;
            }
            case sl::LogLevel::Fatal:
            {
                return FatalColor;
            }
            default:
            {
                return ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };
            }
        }
    };
    auto LevelToIcon = [](sl::LogLevel level)
    {
        switch (level)
        {
            case sl::LogLevel::Trace:
            {
                return ICON_MS_NOTES;
            }
            case sl::LogLevel::Debug:
            {
                return ICON_MS_BUILD;
            }
            case sl::LogLevel::Info:
            {
                return ICON_MS_CHAT;
            }
            case sl::LogLevel::Warn:
            {
                return ICON_MS_WARNING;
            }
            case sl::LogLevel::Error:
            {
                return ICON_MS_CANCEL;
            }
            case sl::LogLevel::Fatal:
            {
                return ICON_MS_BUG_REPORT;
            }
            default:
            {
                return "?";
            }
        }
    };

    // Show log level filter button
    auto LevelButton = [&LogLevelToColor, &LevelToIcon](sl::LogLevel level)
    {
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, LogLevelToColor(level));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, (s_levelFilter & (uint8_t)level) ? 1.0f : 0.5f);
        if (ImGui::Button(LevelToIcon(level)))
        {
            s_levelFilter ^= (uint8_t)level;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    };
    // Show colored log text
    auto LogText = [&LogLevelToColor](const sl::LogInfo &info)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, LogLevelToColor(info.m_level));
        ImGui::TextUnformatted(info.m_content.data());
        ImGui::PopStyleColor();
    };

    if (!ImGui::Begin("Output Log"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Clear"))
    {
        sl::Log::GetLogInfos().clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Copy"))
    {
        ImGui::LogToClipboard();
    }

    LevelButton(sl::LogLevel::Trace);
    LevelButton(sl::LogLevel::Debug);
    LevelButton(sl::LogLevel::Info);
    LevelButton(sl::LogLevel::Warn);
    LevelButton(sl::LogLevel::Error);
    LevelButton(sl::LogLevel::Fatal);

    ImGui::SameLine();
    s_textFilter.Draw("##TextFilter", -ImGui::GetStyle().ScrollbarSize);

    ImGui::Separator();
    auto &logInfos = sl::Log::GetLogInfos();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{ 37.0f / 255.0f, 37.0f / 255.0f, 38.0f / 255.0f, 1.0f });
    ImGui::BeginChild("##LogText", ImVec2{ 0.0f, 0.0f }, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PopStyleColor();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });

    // If any filter is active
    if (s_levelFilter < FullLevelFilter || s_textFilter.IsActive())
    {
        for (size_t i = 0; i < logInfos.size(); ++i)
        {
            const auto &info = logInfos[i];
            if ((s_levelFilter & (uint8_t)info.m_level) && s_textFilter.PassFilter(info.m_content.data()))
            {
                LogText(info);
            }
        }
    }
    else // Without any filter
    {
        ImGuiListClipper clipper;
        clipper.Begin((int)logInfos.size());
        while (clipper.Step())
        {
            for (size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
            {
                LogText(logInfos[i]);
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    // Auto scrolling
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    ImGui::End(); // Output Log
}

void OutputLog::OnRender()
{

}

void OutputLog::EndFrame()
{

}

void OutputLog::OnEvent(sl::Event &event)
{

}
