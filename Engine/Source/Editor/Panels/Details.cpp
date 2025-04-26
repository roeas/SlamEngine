#include "Details.h"

#include "Core/Path.h"
#include "ImGui/ImGuiContext.h"
#include "Panels/ImGuiData.h"
#include "Panels/ImGuiUtils.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"

#include <glm/gtc/type_ptr.hpp>
#include <iconfontcppheaders/IconsMaterialSymbols.h>
#include <imgui/imgui.h>
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

float StartWithText(std::string_view text, float offset = 0.0f)
{
    // Align component parameter widgets
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

    float crtTextSize = ImGui::CalcTextSize(text.data()).x;
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
    ImGui::TextUnformatted(text.data());

    float padding = ImGui::GetStyle().WindowPadding.x;
    float nextOffset = crtOffset + pData->m_maxTextSize + padding;
    ImGui::SameLine(nextOffset);
    ImGui::SetNextItemWidth(-padding);

    return nextOffset;
}

template<typename T>
void DrawComponent(const char *pLabel, auto drawParametersFun)
{
    constexpr ImGuiTreeNodeFlags DefaultTreeFlags =
        ImGuiTreeNodeFlags_Framed |
        ImGuiTreeNodeFlags_AllowOverlap |
        ImGuiTreeNodeFlags_NoTreePushOnOpen |
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_FramePadding |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    static_assert(requires{ drawParametersFun(pData->m_selectedEntity.TryGetComponents<T>()); });

    T *pComponent = pData->m_selectedEntity.TryGetComponents<T>();
    if (!pComponent)
    {
        return;
    }

    ImGui::PushID(nameof::nameof_type<T>().data());
    ImGui::PushFont(sl::ImGuiContext::GetBoldFont());
    bool componentTreeOpen = ImGui::TreeNodeEx(pLabel, DefaultTreeFlags);
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

    // Draw component specific parameters ui.
    if (componentTreeOpen && !removeComponent)
    {
        drawParametersFun(pComponent);
    }

    ImGui::PopID();
    ImGui::Separator();
}

template<typename T>
void AddComponentMenuItem(const char *pLabel)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    static_assert(requires{ pData->m_selectedEntity.TryGetComponents<T>(); });

    if (ImGui::MenuItem(pLabel))
    {
        if (!pData->m_selectedEntity.TryGetComponents<T>())
        {
            pData->m_selectedEntity.AddComponent<T>();
        }
        else
        {
            SL_LOG_WARN("Entity {} already has component {}",
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

    constexpr ImGuiTreeNodeFlags DefaultSubTreeFlags =
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

    // Draw tag component
    DrawComponent<sl::TagComponent>("Tag", [this, pData](sl::TagComponent *pComponent)
    {
        StartWithText("ID");
        ImGui::Text("%d", (uint32_t)pData->m_selectedEntity.GetHandle());

        constexpr size_t BufferSize = 256;
        SL_ASSERT(BufferSize > pComponent->m_name.size(),
            "ImGui ensure that InputText() returns a null-terminated character array,"
            "it also means that character buffer[BufferSize - 1] will be discard.");
        char buffer[BufferSize] = { 0 };
        memcpy(buffer, pComponent->m_name.data(), pComponent->m_name.size());

        StartWithText("Name");
        if (ImGui::InputText("##Name", buffer, BufferSize))
        {
            pComponent->m_name = buffer;
            if (pComponent->m_name.empty())
            {
                pComponent->Reset();
            }
        }
    });

    // Draw transform component
    DrawComponent<sl::TransformComponent>("Transform", [this, pData](sl::TransformComponent *pComponent)
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

        if (auto *pCamera = pData->m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera && cameraMightDirty)
        {
            // If we select a camera entitiy
            pCamera->m_position = pComponent->m_position;
            pCamera->m_rotation = pComponent->m_rotation;
            pCamera->m_isDirty = true;
        }
    });

    // Draw Camera component
    DrawComponent<sl::CameraComponent>("Camera", [this, pData](sl::CameraComponent *pComponent)
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
        if (ImGui::BeginCombo("##Projection", nameof::nameof_enum(pComponent->m_projectionType).data(), ImGuiComboFlags_WidthFitPreview))
        {
            for (size_t i = 0; i < nameof::enum_count<sl::ProjectionType>(); ++i)
            {
                sl::ProjectionType type = (sl::ProjectionType)i;
                if (ImGui::Selectable(nameof::nameof_enum(type).data(), type == pComponent->m_projectionType))
                {
                    pComponent->m_projectionType = type;
                    cameraMightDirty = true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Perspective", DefaultSubTreeFlags))
        {
            float fovDegrees = glm::degrees(pComponent->m_fov);
            StartWithText("FOV");
            if (ImGui::DragFloat("##FOV", &fovDegrees, 0.1f, 1.0f, 120.0f))
            {
                pComponent->m_fov = glm::radians(fovDegrees);
                cameraMightDirty = true;
            }

            StartWithText("Near Plane");
            cameraMightDirty |= ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.001f, 0.01f, 1.0f);

            StartWithText("Far Plane");
            cameraMightDirty |= ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 10.0f, 1.0f, 100000.0f);
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Orthographic", DefaultSubTreeFlags))
        {
            StartWithText("Size");
            cameraMightDirty |= ImGui::DragFloat("##Size", &(pComponent->m_orthoSize), 0.1f, 1.0f, 100000.0f);

            StartWithText("Near Clip");
            cameraMightDirty |= ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 0.1f, -100000.0f, 100000.0f);

            StartWithText("Far Clip");
            cameraMightDirty |= ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 0.1f, -100000.0f, 100000.0f);
        }

        ImGui::Separator();
        if (ImGui::TreeNodeEx("Controller", DefaultSubTreeFlags))
        {
            float rotateSpeedDegrees = glm::degrees(pComponent->m_rotateSpeed);
            StartWithText("Rotate Speed");
            if (ImGui::DragFloat("##RotateSpeed", &rotateSpeedDegrees, 0.001f, 0.001f, 1.0f))
            {
                pComponent->m_rotateSpeed = glm::radians(rotateSpeedDegrees);
            }

            StartWithText("Shift Multiplier");
            ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.01f, 1.0f, 10.0f);

            StartWithText("Scroll Multiplier");
            ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.01f, 0.1f, 10.0f);
        }

        pComponent->m_isDirty |= cameraMightDirty;
    });

    // Draw render component
    DrawComponent<sl::RenderingComponent>("Rendering", [this, pData](sl::RenderingComponent *pComponent)
    {
        if (ImGui::TreeNodeEx("Mesh", DefaultSubTreeFlags))
        {
            StartWithText("Name");
            ImGui::TextUnformatted("");

            StartWithText("Index");
            ImGui::TextUnformatted("");
        }
        if (ImGui::TreeNodeEx("Shader", DefaultSubTreeFlags))
        {
            StartWithText("Name");
            ImGui::TextUnformatted("");
        }
        if (ImGui::TreeNodeEx("Material", DefaultSubTreeFlags))
        {
            StartWithText("Name");
            ImGui::TextUnformatted("");
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
        ImGui::EndPopup();
    }

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
