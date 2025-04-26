#include "AssetBrowser.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Panels/ImGuiData.h"
#include "Panels/ImGuiUtils.h"
#include "Renderer/Texture.h"
#include "Resource/ResourceManager.h"
#include "Utils/ProfilerCPU.h"

#include <iconfontcppheaders/IconsMaterialSymbols.h>
#include <imgui/imgui.h>
#include <stb/stb_image.h>

AssetBrowser::AssetBrowser() : m_fileIconID(sl::StringHash("FileIcon.png")), m_folderIconID(sl::StringHash("FolderIcon.png"))
{
    SL_PROFILE;

    std::unique_ptr<sl::TextureResource> pFileIconResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/FileIcon.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(m_fileIconID, std::move(pFileIconResource));

    std::unique_ptr<sl::TextureResource> pFolderIconResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/FolderIcon.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(m_folderIconID, std::move(pFolderIconResource));
}

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
    SL_PROFILE;

    if (!ImGui::Begin("Asset Browser"))
    {
        ImGui::End();
        return;
    }

    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    pData->m_isMouseHoverAssetBrowser = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);

    // Disable the back button if current path reaches the outermost level
    bool backButtonDisable = pData->m_assetBrowserCrtPath == sl::Path::GetAsset();
    if (backButtonDisable)
    {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button(ICON_MS_ARROW_BACK))
    {
        pData->m_assetBrowserCrtPath = pData->m_assetBrowserCrtPath.parent_path();
    }
    if (backButtonDisable)
    {
        ImGui::EndDisabled();
    }

    // Display the current path
    ImGui::SameLine();
    ImGui::TextUnformatted(pData->m_assetBrowserCrtPath.generic_string().c_str());

    // Select item size
    static size_t s_itemSizeIndex = 2;
    constexpr size_t SizeCount = 5;
    SL_ASSERT(s_itemSizeIndex < SizeCount);
    constexpr std::array<float, SizeCount> BasicColumnSizes =
    {
        48.0f, 64.0f, 80.0f, 96.0f, 112.0f,
    };
    constexpr std::array<const char *, SizeCount> SizeNames =
    {
        "Very Small", "Small", "Medium", "Large", "Very Large",
    };
    constexpr std::array<uint8_t, SizeCount> NameDisplayLines =
    {
        1, 2, 2, 2, 2,
    };

    // Item size combo
    ImGui::SameLine();
    AlignNextWidget(SizeNames[s_itemSizeIndex], 1.0f, -27.0f);
    if (ImGui::BeginCombo("##ItemSizeCombo", SizeNames[s_itemSizeIndex], ImGuiComboFlags_WidthFitPreview))
    {
        for (size_t i = 0; i < SizeCount; ++i)
        {
            if (ImGui::Selectable(SizeNames[i], i == s_itemSizeIndex))
            {
                s_itemSizeIndex = i;
            }
        }
        ImGui::EndCombo();
    }

    // Assets
    ImGui::Separator();
    ImGui::BeginChild("##Assets");
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
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    // Fill item size, I hate these
    uint32_t columnCount = std::max((uint32_t)(available / BasicColumnSizes[s_itemSizeIndex]), 1U);
    float columSize = std::floor(available / (float)columnCount);
    float filledItemSize = columSize - itemSpacing - 1.0f;

    // Show file and folder buttons
    uint32_t columnIndex = 0;
    ImGui::Columns(columnCount, "##AssetBrowserColums", false);
    for (auto it : std::filesystem::directory_iterator(pData->m_assetBrowserCrtPath))
    {
        std::string fileName = it.path().filename().generic_string();
        ImGui::PushID(fileName.data());

        // Multy columns
        if (columnCount > 1)
        {
            // Multy rows
            if (columnIndex >= columnCount)
            {
                columnIndex = 0;
            }
            ImGui::SetColumnWidth(columnIndex++, columSize);
        }

        // Texture resources
        auto *pFileIconResource = sl::ResourceManager::GetTextureResource(m_fileIconID);
        auto *pFolderIconResource = sl::ResourceManager::GetTextureResource(m_folderIconID);
        if (!pFileIconResource || !pFolderIconResource || !pFileIconResource->IsReady() || !pFolderIconResource->IsReady()) [[unlikely]]
        {
            ImGui::PopID();
            ImGui::Columns(1);
            ImGui::EndChild();
            ImGui::End();
            return;
        }

        bool isDirectory = it.is_directory();
        ImTextureID textureID = isDirectory ?
            (ImTextureID)(uint64_t)pFolderIconResource->GetTexture()->GetHandle() :
            (ImTextureID)(uint64_t)pFileIconResource->GetTexture()->GetHandle();

        // ImageButton will adds `style.FramePadding * 2.0f` to provided size
        ImGui::ImageButton("##Icon", textureID,
            ImVec2{ filledItemSize - framePadding.x * 2.0f, filledItemSize - framePadding.y * 2.0f },
            ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

        // Open the folder when double clicking
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && isDirectory)
        {
            pData->m_assetBrowserCrtPath = it.path();
        }

        // Limit the number of lines displayed for a file or folder name
        ImGui::BeginChild("##Name",
            ImVec2{ filledItemSize, ImGui::GetFontSize() * (float)NameDisplayLines[s_itemSizeIndex] },
            ImGuiChildFlags_None, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
        ImGui::TextWrapped(fileName.data());
        ImGui::EndChild();

        ImGui::PopID();
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
    ImGui::EndChild(); // Assets
    ImGui::End(); // Asset Browser
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
