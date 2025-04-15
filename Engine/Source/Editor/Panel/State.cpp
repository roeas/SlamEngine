#include "Panel/State.h"

#include "Core/Log.h"
#include "Renderer/RenderCore.h"
#include "Util/NameOf.hpp"

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
            m_datas[m_offset] = ImVec2(x, y);
            m_offset = (m_offset + 1) % m_maxSize;
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
    ImGui::Begin("Info");

    ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
    ImGui::Separator();
    ShowFPS(deltaTime);
    ShowCost(deltaTime);

    ImGui::End();
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

void State::ShowFPS(float deltaTime)
{
    // In seconds
    static float s_sumTime = 0.0f;
    static ScrollingBuffer s_buffer;

    s_sumTime += deltaTime * 0.001f;
    if (s_sumTime > 1.0f)
    {
        // Waiting for programme to be stable
        s_buffer.AddPoint(s_sumTime, 1000.0f / deltaTime);
    }

    ImGui::TextUnformatted("Frames Per Second:");
    if (ImPlot::BeginPlot("##FPSPanel", ImVec2(-1.0f, 128.0f), ImPlotFlags_CanvasOnly |
        ImPlotFlags_NoInputs | ImPlotFlags_NoFrame))
    {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - 4.0f, s_sumTime, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 500.0f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (!s_buffer.m_datas.empty())
        {
            ImPlot::PlotLine("FPS",
                &s_buffer.m_datas[0].x,
                &s_buffer.m_datas[0].y,
                s_buffer.m_datas.size(),
                0,
                s_buffer.m_offset,
                2 * sizeof(float));
        }

        ImPlot::EndPlot();
    }
}

void State::ShowCost(float deltaTime)
{
    // In milliseconds
    static float s_sumTime = 0.0f;
    static ScrollingBuffer s_buffer;

    s_sumTime += deltaTime;
    if (s_sumTime > 1000.0f)
    {
        // Waiting for programme to be stable
        s_buffer.AddPoint(s_sumTime, deltaTime);
    }

    ImGui::TextUnformatted("Milliseconds Per Frame:");
    if (ImPlot::BeginPlot("##CostPanel", ImVec2(-1.0f, 128.0f),
        ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs | ImPlotFlags_NoFrame))
    {
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_None);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - 4000.0f, s_sumTime, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 50.0f, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (!s_buffer.m_datas.empty())
        {
            ImPlot::PlotShaded("Coast in ms",
                &s_buffer.m_datas[0].x,
                &s_buffer.m_datas[0].y,
                s_buffer.m_datas.size(),
                0, 0,
                s_buffer.m_offset,
                2 * sizeof(float));
        }

        ImPlot::EndPlot();
    }
}
