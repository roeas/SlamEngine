#pragma once

#include "Layer/Layer.h"
#include "Utils/Hash.hpp"

class AssetBrowser : public sl::Layer
{
public:
    AssetBrowser();

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

private:
    sl::StringHashType m_fileIconResourceID;
    sl::StringHashType m_folderIconResourceID;
};
