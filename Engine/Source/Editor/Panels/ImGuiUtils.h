#pragma once

#include <imgui/imgui.h>

inline void AlignNextWidget(const char *pLabel, float align = 0.5f, float customOffset = 0.0f)
{
    float size = ImGui::CalcTextSize(pLabel).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float offset = (avail - size) * align;
    if (offset > 0.0f)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset + customOffset);
    }
}

inline float GetTitleBarSize()
{
    return ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
}
