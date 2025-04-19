#pragma once

#include "Scene/World.h"

struct ImGuiData
{
    sl::Entity m_selectedEntity = 0;
    int m_dockspaceFlag = 0; // ImGuiDockNodeFlags_None
    int m_imguizmoMode = 7; // ImGuizmo::OPERATION::TRANSLATE
    bool m_isMouseInViewport = false;
    bool m_debugImGuiDemo = false;
    bool m_debugImPlotDemo = false;
    bool m_debugImGuizmoState = false;
    bool m_windowShouldClose = false;
};
