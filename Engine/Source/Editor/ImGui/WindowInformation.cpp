#pragma once

#include "ImGui/WindowInformation.h"

#include "Core/Log.h"
#include "ImGui/ImGuiData.h"

#include <imgui/imgui.h>
#include <implot/implot.h>

namespace
{

struct ScrollingBuffer
{
    ScrollingBuffer(int max_size = 1024)
    {
        m_maxSize = max_size;
        m_offset = 0;
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

    int m_maxSize;
    int m_offset;
    ImVector<ImVec2> m_datas;
};

} // namespace

void WindowInformation::OnAttach()
{

}

void WindowInformation::OnDetach()
{

}

void WindowInformation::BeginFrame()
{

}

void WindowInformation::OnUpdate(float deltaTime)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    ImGui::Begin("Info");

    // 1. Infos
    // TODO: ImGui::Text("Backend: %s", nameof::nameof_enum(sl::RenderCore::GetBackend()).data());
    ImGui::Text("Backend:");
    ImGui::Separator();

    // 2. FPS
    static float s_sumTime = 0.0f; // Stores in seconds
    static ScrollingBuffer s_FPSBuffer;
    static ScrollingBuffer s_coastBuffer;

    constexpr float History = 5.0f;
    constexpr float Delay = 1.0f;

    s_sumTime += deltaTime / 1000.0f;
    if (s_sumTime > Delay)
    {
        // Waiting for programme to be stable
        s_FPSBuffer.AddPoint(s_sumTime, 1000.0f / deltaTime);
        s_coastBuffer.AddPoint(s_sumTime, deltaTime / 1000.0f);
    }

    constexpr ImPlotFlags PlotFlag = ImPlotFlags_CanvasOnly;
    constexpr ImPlotAxisFlags AxisFlagX = ImPlotAxisFlags_NoTickLabels;
    constexpr ImPlotAxisFlags AxisFlagY = ImPlotAxisFlags_AutoFit;

    ImGui::TextUnformatted("FPS:");
    if (ImPlot::BeginPlot("FPS Window", ImVec2(-1.0f, 150.0f), PlotFlag))
    {
        ImPlot::SetupAxes(nullptr, nullptr, AxisFlagX, AxisFlagY);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - History, s_sumTime, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (s_sumTime > Delay)
        {
            ImPlot::PlotLine("FPS",
                &s_FPSBuffer.m_datas[0].x,
                &s_FPSBuffer.m_datas[0].y,
                s_FPSBuffer.m_datas.size(),
                0,
                s_FPSBuffer.m_offset,
                2 * sizeof(float));
        }
        ImPlot::EndPlot();
    }

    ImGui::TextUnformatted("Cost:");
    if (ImPlot::BeginPlot("Cost window", ImVec2(-1.0f, 150.0f), PlotFlag))
    {
        ImPlot::SetupAxes(nullptr, nullptr, AxisFlagX, AxisFlagY);
        ImPlot::SetupAxisLimits(ImAxis_X1, s_sumTime - History, s_sumTime, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
        if (s_sumTime > Delay)
        {
            ImPlot::PlotShaded("Coast in ms",
                &s_coastBuffer.m_datas[0].x,
                &s_coastBuffer.m_datas[0].y,
                s_coastBuffer.m_datas.size(),
                0, 0,
                s_coastBuffer.m_offset,
                2 * sizeof(float));
        }
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void WindowInformation::OnRender()
{

}

void WindowInformation::EndFrame()
{

}

void WindowInformation::OnEvent(sl::Event &event)
{

}
