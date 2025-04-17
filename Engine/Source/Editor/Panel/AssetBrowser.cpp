#include "Panel/AssetBrowser.h"

#include "Core/Path.h"
#include "ImGui/IconsMaterialSymbols.h"
#include "Panel/ImGuiData.h"
#include "Renderer/Texture.h"

#include <imgui/imgui.h>
#include <stb/stb_image.h>

void AssetBrowser::OnAttach()
{

}

void AssetBrowser::OnDetach()
{

}

void AssetBrowser::BeginFrame()
{

}

void AssetBrowser::OnUpdate(float deltaTime)
{
    // TMP
    static bool s_first = true;
    static sl::Texture2D *pFileTexture;
    static sl::Texture2D *pFolderTexture;
    if (s_first)
    {
        s_first = false;
        {
            int width, height, channel;
            stbi_set_flip_vertically_on_load(1);
            auto *pData = stbi_load(sl::Path::FromeAsset("Texture/FileIcon.png").data(), &width, &height, &channel, 4);
            pFileTexture = sl::Texture2D::Create(width, height, sl::TextureFormat::RGBA8, true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR, pData);
            stbi_image_free(pData);
        }
        {
            int width, height, channel;
            stbi_set_flip_vertically_on_load(1);
            auto *pData = stbi_load(sl::Path::FromeAsset("Texture/FolderIcon.png").data(), &width, &height, &channel, 4);
            pFolderTexture = sl::Texture2D::Create(width, height, sl::TextureFormat::RGBA8, true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR, pData);
            stbi_image_free(pData);
        }
    }

    if (!ImGui::Begin("Asset Browser"))
    {
        ImGui::End();
        return;
    }

    std::string crtPath = m_assetBrowserCrtPath.generic_string();
    
    // Disable the back button if the current path reaches the outermost path
    const bool backButtonDisabled = !sl::Path::Contain(sl::Path::GetAsset(), crtPath);
    if (backButtonDisabled)
    {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button(ICON_MS_ARROW_BACK))
    {
        m_assetBrowserCrtPath = m_assetBrowserCrtPath.parent_path();
    }
    if (backButtonDisabled)
    {
        ImGui::EndDisabled();
    }

    // Display the current path
    ImGui::SameLine();
    ImGui::TextUnformatted(crtPath.c_str());

    // Select item size
    static size_t s_itemSizeIndex = 2;
    constexpr size_t SizeCount = 5;
    float DPIFactor = 16.0f;
    const std::array<float, SizeCount> BasicColumSizes =
    {
        DPIFactor * 2.0f, DPIFactor * 3.0f, DPIFactor * 4.0f, DPIFactor * 5.0f, DPIFactor * 6.0f,
    };
    constexpr std::array<const char *, SizeCount> SizeNames =
    {
        "Very Small", "Small", "Medium", "Large", "Very Large",
    };
    constexpr std::array<uint8_t, SizeCount> NameDisplayLines =
    {
        1, 2, 3, 4, 5,
    };

    ImGui::SameLine();
    // AlignNextWidget(SizeNames.at(s_itemSizeIndex), 1.0f, -25.0f);
    // 25.0f is approximately the size of the triangle of the combo widget
    if (ImGui::BeginCombo("##ItemSizeCombo", SizeNames.at(s_itemSizeIndex), ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t i = 0; i < SizeCount; ++i)
        {
            if (ImGui::Selectable(SizeNames.at(i), i == s_itemSizeIndex))
            {
                s_itemSizeIndex = i;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    float available = ImGui::GetContentRegionAvail().x;
    float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
    ImVec2 framePadding = ImGui::GetStyle().FramePadding;

    // To avoid scroll bar flickering
    if (ImGui::GetScrollMaxY() < 1.0f)
    {
        available -= ImGui::GetStyle().ScrollbarSize;
    }

    if (available <= 0.0f)
    {
        ImGui::End();
        return;
    }

    available += itemSpacing;
    uint32_t columCount = (uint32_t)available / (uint32_t)BasicColumSizes.at(s_itemSizeIndex);
    if (columCount < 1)
    {
        columCount = 1;
    }

    float columSize = std::floor(available / (float)columCount);
    float filledItemSize = columSize - itemSpacing - 1.0f;

    // Show files and folders under current path
    uint32_t columIndex = 0;
    ImGui::Columns(columCount, "##AssetBrowserColums", false);
    for (const auto &it : std::filesystem::directory_iterator(m_assetBrowserCrtPath))
    {
        if (columCount > 1)
        {
            if (columIndex >= columCount)
            {
                columIndex = 0;
            }
            ImGui::SetColumnWidth(columIndex++, columSize);
        }

        std::string fileName = it.path().filename().generic_string();
        ImGui::PushID(fileName.data());

        bool isDirectory = it.is_directory();
        ImTextureID textureID = isDirectory ?
            (ImTextureID)(uint64_t)pFolderTexture->GetHandle() :
            (ImTextureID)(uint64_t)pFileTexture->GetHandle();
        // ImageButton will adds `style.FramePadding * 2.0f` to provided size
        ImGui::ImageButton("##Icon", textureID,
            ImVec2{ filledItemSize - framePadding.x * 2.0f, filledItemSize - framePadding.y * 2.0f },
            ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

        // Open the folder when double clicking on the folder
        if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            m_assetBrowserCrtPath = it.path();
        }

        // Limit the number of lines displayed for a file or folder name
        ImGui::BeginChild(
            "##Text",
            ImVec2{ filledItemSize, ImGui::GetFontSize() * (float)NameDisplayLines.at(s_itemSizeIndex) },
            ImGuiChildFlags_None,
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration);
        ImGui::TextWrapped(fileName.c_str());
        ImGui::EndChild();

        ImGui::PopID();
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::End();
}

void AssetBrowser::OnRender()
{

}

void AssetBrowser::EndFrame()
{

}

void AssetBrowser::OnEvent(sl::Event &event)
{

}
