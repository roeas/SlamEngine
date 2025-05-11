#include "Details.h"

#include "Core/Path.h"
#include "ImGui/ImGuiContext.h"
#include "Panels/ImGuiData.h"
#include "Panels/ImGuiUtils.h"
#include "Renderer/Texture.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"

#include <glm/gtc/type_ptr.hpp>
#include <iconfontcppheaders/IconsMaterialSymbols.h>
#include <imgui/imgui.h>
#include <magic_enum/magic_enum.hpp>
#include <nameof/nameof.hpp>

namespace
{

glm::vec3 RotationMod(const glm::vec3 &v)
{
    glm::vec3 ret;
    for (glm::length_t i = 0; i < 3; ++i)
    {
        ret[i] = std::fmod(v[i], 360.0f);
        if (ret[i] < -180.0f)
        {
            ret[i] += 360.0f;
        }
        else if (ret[i] > 180.0f)
        {
            ret[i] -= 360.0f;
        }
    }

    return ret;
}

float StartWithText(const char *pText, float offset = 0.0f)
{
    // Align component parameters widgets
    static sl::Entity s_crtEntity;
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (s_crtEntity != pData->m_selectedEntity)
    {
        /*
         * `ImGui::CalcTextSize("Rotation").x == 56.0f`
         * `ImGui::CalcTextSize("Position").x == 56.0f`
         * Just a little trick to avoid tag component flickering,
         * as we known every entity must hold both tag and transform component.
         */
        pData->m_maxTextSize = 56.0f;
        s_crtEntity = pData->m_selectedEntity;
    }

    float crtTextSize = ImGui::CalcTextSize(pText).x;
    float crtOffset;
    if (offset > 0.0f)
    {
        crtOffset = offset;
    }
    else
    {
        pData->m_maxTextSize = std::max(pData->m_maxTextSize, crtTextSize);
        crtOffset = ImGui::GetStyle().IndentSpacing;
    }

    ImGui::SetCursorPosX(crtOffset);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(pText);

    float padding = ImGui::GetStyle().WindowPadding.x;
    float nextOffset = crtOffset + pData->m_maxTextSize + padding;
    ImGui::SameLine(nextOffset);
    ImGui::SetNextItemWidth(-padding);

    return nextOffset;
}

template<typename T>
void DrawComponent(const char *pLabel, auto drawParametersFun)
{
    constexpr ImGuiTreeNodeFlags TreeFlags =
        ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_AllowOverlap |
        ImGuiTreeNodeFlags_NoTreePushOnOpen |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    T *pComponent = pData->m_selectedEntity.TryGetComponents<T>();
    if (!pComponent)
    {
        return;
    }

    ImGui::PushID(nameof::nameof_type<T>().data());
    ImGui::PushFont(sl::ImGuiContext::GetBoldFont());
    bool componentTreeOpen = ImGui::TreeNodeEx(pLabel, TreeFlags);
    ImGui::PopFont();

    // Draw component menu button
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    AlignNextWidget(ICON_MS_MORE_VERT, 1.0f, ImGui::GetStyle().FramePadding.x);
    if (ImGui::Button(ICON_MS_MORE_VERT))
    {
        ImGui::OpenPopup("##ComponentPopup");
    }
    ImGui::PopStyleColor();

    bool removeComponent = false;
    if (ImGui::BeginPopup("##ComponentPopup"))
    {
        if constexpr (requires{ pComponent->Reset(); })
        {
            if (ImGui::MenuItem("Reset Component"))
            {
                pComponent->Reset();

                // Camera entity should be affected by transform component
                if constexpr (std::is_same_v<T, sl::TransformComponent>)
                {
                    if (auto pCamera = pData->m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera)
                    {
                        const auto &transform = pData->m_selectedEntity.GetComponents<sl::TransformComponent>();
                        pCamera->m_position = transform.m_position;
                        pCamera->m_rotation = transform.m_rotation;
                        pCamera->m_isDirty = true;
                    }
                }
            }
        }

        bool canRemove = true;
        // Not allowed to remove #TagComponent and #TransformComponent
        if constexpr (std::is_same_v<T, sl::TagComponent> || std::is_same_v<T, sl::TransformComponent>)
        {
            canRemove = false;
        }
        // Not allowed to remove #CameraComponent if it is a main camera
        else if (std::is_same_v<T, sl::CameraComponent>)
        {
            if (pData->m_selectedEntity.GetComponents<sl::CameraComponent>().m_isMainCamera)
            {
                canRemove = false;
            }
        }

        if (canRemove && ImGui::MenuItem("Remove Component"))
        {
            pData->m_selectedEntity.RemoveComponent<T>();
            pData->m_maxTextSize = 56.0f;
            removeComponent = true;
        }

        ImGui::EndPopup();
    }

    // Draw component parameters widgets
    if (componentTreeOpen && !removeComponent)
    {
        drawParametersFun(pComponent);
    }

    ImGui::PopID();
}

template<typename T>
void AddComponentMenuItem(const char *pLabel)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    if (ImGui::MenuItem(pLabel))
    {
        if (!pData->m_selectedEntity.TryGetComponents<T>())
        {
            pData->m_selectedEntity.AddComponent<T>();
        }
        else
        {
            SL_LOG_WARN("Entity {} already holds {} component",
                (uint32_t)pData->m_selectedEntity.GetHandle(), nameof::nameof_type<T>());
        }
    }
}

} // namespace

void Details::OnAttach()
{

}

void Details::OnDetach()
{

}

void Details::BeginFrame()
{

}

void Details::OnUpdate(float deltaTime)
{
    SL_PROFILE;

    constexpr ImGuiTreeNodeFlags SubTreeFlags =
        ImGuiTreeNodeFlags_NoTreePushOnOpen |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (!ImGui::Begin("Details") || !pData->m_selectedEntity.IsValid())
    {
        ImGui::End();
        return;
    }
    ImGui::PushID((void *)(uint64_t)(uint32_t)pData->m_selectedEntity.GetHandle());

    // Draw tag component
    DrawComponent<sl::TagComponent>("Tag", [pData](sl::TagComponent *pComponent)
    {
        StartWithText("ID");
        ImGui::Text("%d", (uint32_t)pData->m_selectedEntity.GetHandle());

        constexpr size_t BufferSize = 256;
        static char s_buffer[BufferSize] = { 0 };

        auto nameSize = pComponent->m_name.size();
        SL_ASSERT(BufferSize > nameSize,
            "ImGui ensure that InputText() returns a null-terminated character array,"
            "it also means that character s_buffer[BufferSize - 1] will be discard.");
        memcpy(s_buffer, pComponent->m_name.data(), nameSize);
        memset(s_buffer + nameSize, 0, BufferSize - nameSize);

        StartWithText("Name");
        if (ImGui::InputText("##Name", s_buffer, BufferSize))
        {
            pComponent->m_name = s_buffer;
            if (pComponent->m_name.empty())
            {
                pComponent->Reset();
            }
        }
    });

    // Draw transform component
    DrawComponent<sl::TransformComponent>("Transform", [pData](sl::TransformComponent *pComponent)
    {
        bool cameraMightDirty = false;

        StartWithText("Position");
        cameraMightDirty |= ImGui::DragFloat3("##Position", glm::value_ptr(pComponent->m_position), 0.1f);

        StartWithText("Rotation");
        glm::vec3 ratationDegrees = RotationMod(pComponent->GetRotationDegrees());
        if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegrees), 0.1f))
        {
            pComponent->SetRotationDegrees(ratationDegrees);
            cameraMightDirty = true;
        }

        StartWithText("Scale");
        ImGui::DragFloat3("##Scale", glm::value_ptr(pComponent->m_scale), 0.1f);

        // If we select a camera entitiy
        if (auto *pCamera = pData->m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera && cameraMightDirty)
        {
            pCamera->m_position = pComponent->m_position;
            pCamera->m_rotation = pComponent->m_rotation;
            pCamera->m_isDirty = true;
        }
    });

    // Draw Camera component
    DrawComponent<sl::CameraComponent>("Camera", [pData](sl::CameraComponent *pComponent)
    {
        bool cameraMightDirty = false;

        StartWithText("Main Camera");
        bool isMainCamera = pComponent->m_isMainCamera;
        if (ImGui::Checkbox("##MainCameraCheckBox", &isMainCamera))
        {
            if (isMainCamera)
            {
                auto view = sl::World::GetRegistry().view<sl::CameraComponent>();
                for (auto entity : view)
                {
                    view.get<sl::CameraComponent>(entity).m_isMainCamera = false;
                }
                pComponent->m_isMainCamera = true;
                cameraMightDirty = true;
            }
            else
            {
                SL_LOG_WARN("Main camera must exist!");
            }
        }

        StartWithText("Projection");
        if (ImGui::BeginCombo("##Projection", magic_enum::enum_name(pComponent->m_projectionType).data(), ImGuiComboFlags_WidthFitPreview))
        {
            for (size_t i = 0; i < magic_enum::enum_count<sl::ProjectionType>(); ++i)
            {
                sl::ProjectionType type = (sl::ProjectionType)i;
                if (ImGui::Selectable(magic_enum::enum_name(type).data(), type == pComponent->m_projectionType))
                {
                    pComponent->m_projectionType = type;
                    cameraMightDirty = true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Perspective", SubTreeFlags))
        {
            StartWithText("FOV");
            float fovDegrees = glm::degrees(pComponent->m_fov);
            if (ImGui::DragFloat("##FOV", &fovDegrees, 1.0f, 1.0f, 120.0f))
            {
                pComponent->m_fov = glm::radians(fovDegrees);
                cameraMightDirty = true;
            }

            StartWithText("Near Plane");
            cameraMightDirty |= ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.1f, 0.1f, 1.0f);

            StartWithText("Far Plane");
            cameraMightDirty |= ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 1.0f, 1.0f, 10000.0f);
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Orthographic", SubTreeFlags))
        {
            StartWithText("Size");
            cameraMightDirty |= ImGui::DragFloat("##Size", &(pComponent->m_orthoSize), 1.0f, 1.0f, 10000.0f);

            StartWithText("Near Clip");
            cameraMightDirty |= ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 1.0f, -10000.0f, 10000.0f);

            StartWithText("Far Clip");
            cameraMightDirty |= ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 1.0f, -10000.0f, 10000.0f);
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Controller", SubTreeFlags))
        {
            float rotateSpeedDegrees = glm::degrees(pComponent->m_rotateSpeed);
            StartWithText("Rotate Speed");
            if (ImGui::DragFloat("##RotateSpeed", &rotateSpeedDegrees, 0.001f, 0.001f, 1.0f))
            {
                pComponent->m_rotateSpeed = glm::radians(rotateSpeedDegrees);
            }

            StartWithText("Shift Multiplier");
            ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.1f, 0.1f, 10.0f);

            StartWithText("Scroll Multiplier");
            ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.1f, 0.1f, 10.0f);
        }

        pComponent->m_isDirty |= cameraMightDirty;
    });

    // Draw render component
    DrawComponent<sl::RenderingComponent>("Rendering", [pData](sl::RenderingComponent *pComponent)
    {
        if (ImGui::TreeNodeEx("Material", SubTreeFlags))
        {
            if (auto *pMaterialResource = sl::ResourceManager::GetMaterialResource(pComponent->m_materialResourceID); pMaterialResource)
            {
                StartWithText("Name");
                ImGui::TextUnformatted(pMaterialResource->GetName().data());

                auto DrawPropertyGroup = [](const char *pLabel, auto &propertyGroup)
                {
                    ImGui::PushID(pLabel);
                    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

                    float offset = StartWithText(pLabel);
                    constexpr ImVec2 TextureSize{ 64.0f, 64.0f };
                    if (auto *pTexture = sl::ResourceManager::GetTextureResource(propertyGroup.m_textureID); pTexture)
                    {
                        ImGui::Image((ImTextureID)pTexture->GetTexture()->GetHandle(), TextureSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
                        ImGui::SameLine(offset + pData->m_maxTextSize);
                        ImGui::TextUnformatted(pTexture->GetName().data());
                    }
                    else
                    {
                        // Draw a placeholder texture when required texture resource not exist
                        ImGui::Image((ImTextureID)sl::ResourceManager::GetTextureResource(sl::StringHash("NoResource Texture"))->GetTexture()->GetHandle(),
                            TextureSize, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
                    }

                    StartWithText("Use texture", offset);
                    ImGui::Checkbox("##UseTexture", &propertyGroup.m_useTexture);
                    StartWithText("Factor", offset);
                    if constexpr (std::same_as<decltype(propertyGroup.m_factor), glm::vec3>)
                    {
                        ImGui::DragFloat3("##Factor", glm::value_ptr(propertyGroup.m_factor), 0.1f, 0.0f, 1000.0f);
                    }
                    else if constexpr (std::same_as<decltype(propertyGroup.m_factor), glm::vec2>)
                    {
                        ImGui::DragFloat2("##Factor", glm::value_ptr(propertyGroup.m_factor), 0.1f, 0.0f, 1000.0f);
                    }
                    else if constexpr (std::same_as<decltype(propertyGroup.m_factor), float>)
                    {
                        ImGui::DragFloat("##Factor", &propertyGroup.m_factor, 0.1f, 0.0f, 1000.0f);
                    }
                    StartWithText("Offset", offset);
                    ImGui::DragFloat2("##Offset", glm::value_ptr(propertyGroup.m_offset));
                    StartWithText("Scale", offset);
                    ImGui::DragFloat2("##Scale", glm::value_ptr(propertyGroup.m_scale));
                    StartWithText("Rotation", offset);
                    ImGui::DragFloat("##Rotation", &propertyGroup.m_rotation);

                    ImGui::PopID();
                };

                ImGui::Separator();
                DrawPropertyGroup("Albedo", pMaterialResource->GetAlbedoPropertyGroup());
                ImGui::Separator();
                DrawPropertyGroup("Normal", pMaterialResource->GetNormalPropertyGroup());
                ImGui::Separator();
                DrawPropertyGroup("Occlusion", pMaterialResource->GetOcclusionPropertyGroup());
                ImGui::Separator();
                DrawPropertyGroup("Roughness", pMaterialResource->GetRoughnessPropertyGroup());
                ImGui::Separator();
                DrawPropertyGroup("Metallic", pMaterialResource->GetMetallicPropertyGroup());
                ImGui::Separator();
                DrawPropertyGroup("Emissive", pMaterialResource->GetEmissivePropertyGroup());

                ImGui::Separator();
                StartWithText("TwoSide");
                ImGui::Checkbox("##TwoSide", &pMaterialResource->GetTwoSide());
            }
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Shader", SubTreeFlags))
        {
            if (auto *pShaderResource = sl::ResourceManager::GetShaderResource(pComponent->m_shaderResourceID); pShaderResource)
            {
                StartWithText("Name");
                ImGui::TextUnformatted(pShaderResource->GetName().data());
            }
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Mesh", SubTreeFlags))
        {
            if (auto *pMeshResource = sl::ResourceManager::GetMeshResource(pComponent->m_meshResourceID); pMeshResource)
            {
                StartWithText("Name");
                ImGui::TextUnformatted(pMeshResource->GetName().data());

                StartWithText("Vertex");
                ImGui::Text("%i", pMeshResource->GetVerticesCount());

                StartWithText("Index");
                ImGui::Text("%i", pMeshResource->GetIndicesCount());
            }
        }
    });

    // Draw sky component
    DrawComponent<sl::SkyComponent>("Sky", [pData](sl::SkyComponent *pComponent)
    {
        if (ImGui::TreeNodeEx("Texture", SubTreeFlags))
        {
            if (auto *pTextureResource = sl::ResourceManager::GetTextureResource(pComponent->m_textureResourceID); pTextureResource)
            {
                StartWithText("Sky");
                ImGui::TextUnformatted(pTextureResource->GetName().data());
            }

            StartWithText("Radiance");
            ImGui::TextUnformatted("");

            StartWithText("Irradiance");
            ImGui::TextUnformatted("");
        }
        
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Shader", SubTreeFlags))
        {
            if (auto *pShaderResource = sl::ResourceManager::GetShaderResource(pComponent->m_shaderResourceID); pShaderResource)
            {
                StartWithText("Name");
                ImGui::TextUnformatted(pShaderResource->GetName().data());
            }
        }
        
        ImGui::Separator();
        if (ImGui::TreeNodeEx("Mesh", SubTreeFlags))
        {
            if (auto *pMeshResource = sl::ResourceManager::GetMeshResource(pComponent->m_meshResourceID); pMeshResource)
            {
                StartWithText("Name");
                ImGui::TextUnformatted(pMeshResource->GetName().data());

                StartWithText("Vertex");
                ImGui::Text("%i", pMeshResource->GetVerticesCount());

                StartWithText("Index");
                ImGui::Text("%i", pMeshResource->GetIndicesCount());
            }
        }
    });

    // Add component button
    AlignNextWidget("Add Component");
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("##AddComponentPopup");
    }
    if (ImGui::BeginPopup("##AddComponentPopup"))
    {
        AddComponentMenuItem<sl::CameraComponent>("Camera");
        AddComponentMenuItem<sl::RenderingComponent>("Rendering");
        ImGui::EndPopup();
    }

    ImGui::PopID();
    ImGui::End(); // Details
}

void Details::OnRender()
{

}

void Details::EndFrame()
{

}

void Details::OnEvent(sl::Event &event)
{

}
