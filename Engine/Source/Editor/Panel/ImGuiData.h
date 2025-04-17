#pragma once

#include "Scene/World.h"

struct ImGuiData
{
    sl::Entity m_selectedEntity = 0;
    int m_dockspaceFlag = 0; // ImGuiDockNodeFlags_None
    bool m_isMouseInViewport = false;
    bool m_debugImGuiDemo = false;
    bool m_debugImPlotDemo = false;
};
