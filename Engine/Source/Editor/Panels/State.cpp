#include "State.h"

#include "Core/Log.h"
#include "Renderer/RenderCore.h"
#include "Utils/NameOf.hpp"
#include "Utils/ProfilerCPU.h"

#include <imgui/imgui.h>
#include <implot/implot.h>

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

    ImGui::Begin("State");

    constexpr float WindowTime = 0.5f; // In seconds
    static float s_sumFrame = 0.0f;
    static float s_sumTime = 0.0f; // In seconds
    static float s_plotSumTime = 0.0f; // In seconds
    static ScrollingBuffer s_buffer;

    float avgFPS = 0.0f;
    float crtFPS = 1000.0f / deltaTime;

    ++s_sumFrame;
    s_sumTime += deltaTime * 0.001f;
    s_plotSumTime += deltaTime * 0.001f;
    if (s_sumTime >= WindowTime)
    {
        avgFPS = s_sumFrame / s_sumTime;
        s_buffer.AddPoint(s_plotSumTime, avgFPS);
        s_sumFrame = 0.0f;
        s_sumTime = 0.0f;
    }

    ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
    ImGui::Text("Average FPS: %i", (int)std::round(avgFPS));
    ImGui::Text("FPS: %i", (int)std::round(crtFPS));
    ImGui::Text("Cost: %i ms", (int)std::round(deltaTime));

    SL_PROFILE_PLOT("Frame Per Second", crtFPS);

    ImGui::Separator();
    ImGui::TextUnformatted("Average FPS:");
    if (ImPlot::BeginPlot("##FPSPanel", ImVec2(-1.0f, 128.0f),
        ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame))
    {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_plotSumTime - 5.0f, s_plotSumTime - WindowTime, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 200.0f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (!s_buffer.m_datas.empty()) [[likely]]
        {
            ImPlot::PlotLine("FPS",
                &s_buffer.m_datas[0].x,
                &s_buffer.m_datas[0].y,
                s_buffer.m_datas.size(),
                ImPlotLineFlags_SkipNaN | ImPlotLineFlags_Shaded,
                s_buffer.m_offset,
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
