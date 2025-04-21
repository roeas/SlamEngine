#pragma once

#include "Scene/World.h"

#include <filesystem>

struct ImGuiData
{
    std::filesystem::path m_assetBrowserCrtPath = SL_ASSET_PATH;
    sl::Entity m_selectedEntity = 0; // Main camera entity
    uint32_t m_dockspaceFlag = 0; // ImGuiDockNodeFlags_None
    int m_imguizmoMode = 7; // ImGuizmo::OPERATION::TRANSLATE
    float m_maxTextSize = 56.0f; // ImGui::CalcTextSize("Position").x
    bool m_isMouseHoverViewport = false;
    bool m_isMouseHoverAssetBrowser = false;
    bool m_debugImGuiDemo = false;
    bool m_debugImPlotDemo = false;
    bool m_debugImGuizmoState = false;
    bool m_debugImGuizmoIsOver = false;
    bool m_debugImGuizmoIsUsing = false;
    bool m_debugImGuizmoIsUsingViewManipulate = false;
    bool m_debugImGuizmoIsViewManipulateHovered = false;
    bool m_debugImGuizmoIsUsingAny = false;
    bool m_windowShouldClose = false;
};
