#pragma once

#include "Layer/Layer.h"
#include "Core/Path.h"

class AssetBrowser : public sl::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

private:
    std::filesystem::path m_assetBrowserCrtPath = sl::Path::GetAsset();
};
