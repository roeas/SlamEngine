#include "State.h"

#include "Core/Log.h"
#include "Renderer/RenderCore.h"
#include "Utils/ProfilerCPU.h"

#include <imgui/imgui.h>
#include <implot/implot.h>
#include <magic_enum/magic_enum.hpp>

namespace
{

struct ScrollingBuffer
{
    ScrollingBuffer(int maxSize = 1024) : m_maxSize(maxSize), m_offset(0)
    {
        m_datas.reserve(m_maxSize);
    }

    void AddPoint(float x, float y)
    {
        if (m_datas.size() < m_maxSize)
        {
            m_datas.push_back(ImVec2(x, y));
        }
        else
        {
            m_datas[m_offset++] = ImVec2(x, y);
            m_offset = m_offset >= m_maxSize ? 0 : m_offset;
        }
    }

    void Erase()
    {
        if (m_datas.size() > 0)
        {
            m_datas.shrink(0);
            m_offset = 0;
        }
    }

    ImVector<ImVec2> m_datas;
    int m_maxSize;
    int m_offset;
};

} // namespace

void State::OnAttach()
{

}

void State::OnDetach()
{

}

void State::BeginFrame()
{

}

void State::OnUpdate(float deltaTime)
{
    SL_PROFILE;

    constexpr float PlotWindow = 0.5f;

    static float s_plotSumTime = 0.0f;
    static float s_sumFrame = 0.0f;
    static float s_windowSumTime = 0.0f;
    static float s_avgFPS = 0.0f;
    static ScrollingBuffer s_avgBuffer;
    static ScrollingBuffer s_crtBuffer;

    s_plotSumTime += deltaTime * 0.001f;

    // Current
    float crtFPS = 1000.0f / deltaTime;
    s_crtBuffer.AddPoint(s_plotSumTime, crtFPS);
    SL_PROFILE_PLOT("Frame Per Second", crtFPS);

    // Average
    ++s_sumFrame;
    s_windowSumTime += deltaTime * 0.001f;
    if (s_windowSumTime > PlotWindow)
    {
        s_avgFPS = s_sumFrame / s_windowSumTime;
        s_avgBuffer.AddPoint(s_plotSumTime, s_avgFPS);
        s_sumFrame = 0.0f;
        s_windowSumTime = 0.0f;
    }

    if (!ImGui::Begin("State"))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Backend: %s", magic_enum::enum_name(sl::RenderCore::GetBackend()).data());
    ImGui::Text("Average FPS: %i", (int)std::round(s_avgFPS));
    ImGui::Text("Current FPS: %i", (int)std::round(crtFPS));

    ImGui::Separator();
    ImGui::TextUnformatted("Average FPS:");
    if (ImPlot::BeginPlot("##AverageFPSPanel", ImVec2(-1.0f, 128.0f),
        ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame))
    {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_plotSumTime - 5.0f, s_plotSumTime - PlotWindow, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 500.0f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (!s_avgBuffer.m_datas.empty())
        {
            ImPlot::PlotLine("Average FPS",
                &s_avgBuffer.m_datas[0].x,
                &s_avgBuffer.m_datas[0].y,
                s_avgBuffer.m_datas.size(),
                ImPlotLineFlags_SkipNaN | ImPlotLineFlags_Shaded,
                s_avgBuffer.m_offset,
                2 * sizeof(float));
        }

        ImPlot::EndPlot();
    }

    ImGui::TextUnformatted("Current FPS:");
    if (ImPlot::BeginPlot("##CurrentFPSPanel", ImVec2(-1.0f, 128.0f),
        ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame))
    {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_plotSumTime - 5.0f, s_plotSumTime - PlotWindow, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 500.0f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (!s_crtBuffer.m_datas.empty())
        {
            ImPlot::PlotLine("Current FPS",
                &s_crtBuffer.m_datas[0].x,
                &s_crtBuffer.m_datas[0].y,
                s_crtBuffer.m_datas.size(),
                ImPlotLineFlags_SkipNaN | ImPlotLineFlags_Shaded,
                s_crtBuffer.m_offset,
                2 * sizeof(float));
        }

        ImPlot::EndPlot();
    }

    ImGui::End(); // State
}

void State::OnRender()
{

}

void State::EndFrame()
{

}

void State::OnEvent(sl::Event &event)
{

}
