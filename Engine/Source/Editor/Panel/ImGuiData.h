#pragma once

#include "Core/Path.h"
#include "Scene/World.h"

#include <filesystem>

struct ImGuiData
{
    std::filesystem::path m_assetBrowserCrtPath = sl::Path::GetAsset();
    sl::Entity m_selectedEntity = 0;
    float m_maxTextSize = 0.0f;
    int m_dockspaceFlag = 0; // ImGuiDockNodeFlags_None
    bool m_debugImGuiDemo = false;
    bool m_debugImPlotDemo = false;
};
