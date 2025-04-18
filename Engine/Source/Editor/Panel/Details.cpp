#include "Panel/Details.h"

#include "ImGui/IconsMaterialSymbols.h"
#include "ImGui/ImGuiContext.h"
#include "Panel/ImGuiData.h"
#include "Scene/World.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <nameof/nameof.hpp>

namespace
{

constexpr ImGuiTreeNodeFlags DefaultSubTreeFlags =
    ImGuiTreeNodeFlags_NoTreePushOnOpen |
    ImGuiTreeNodeFlags_NoAutoOpenOnLog |
    ImGuiTreeNodeFlags_DefaultOpen |
    ImGuiTreeNodeFlags_OpenOnDoubleClick |
    ImGuiTreeNodeFlags_OpenOnArrow |
    ImGuiTreeNodeFlags_SpanAvailWidth;

constexpr ImGuiTreeNodeFlags DefaultTreeFlags =
    DefaultSubTreeFlags |
    ImGuiTreeNodeFlags_Framed |
    ImGuiTreeNodeFlags_AllowOverlap;

glm::vec3 RotationModAndRepeat(const glm::vec3 &v)
{
    glm::vec3 ret = v;
    for (glm::length_t i = 0; i < 3; ++i)
    {
        while (ret[i] < 180.0f)
        {
            ret[i] += 360.0f;
        }
        while (ret[i] > 180.0f)
        {
            ret[i] -= 360.0f;
        }
    }

    return ret;
}

void AlignNextWidget(const char *pLabel, float align = 0.5f, float customOffset = 0.0f)
{
    float size = ImGui::CalcTextSize(pLabel).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float offset = (avail - size) * align;
    if (offset > 0.0f)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset + customOffset);
    }
}

template<typename T>
void DrawComponent(const char *pLabel, auto drawParameters)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    static_assert(requires{ drawParameters(pData->m_selectedEntity.TryGetComponents<T>()); });

    T *pComponent = pData->m_selectedEntity.TryGetComponents<T>();
    if (!pComponent)
    {
        return;
    }

    ImGui::PushID(nameof::nameof_type<T>().data());

    // Draw tree node
    ImGui::PushFont(sl::ImGuiContext::GetBoldFont());
    bool componentTreeOpen = ImGui::TreeNodeEx(pLabel, DefaultTreeFlags, pLabel);
    ImGui::PopFont();

    // Draw component menu button
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    AlignNextWidget(ICON_MS_MORE_VERT, 1.0f, ImGui::GetStyle().WindowPadding.x * 0.5f);
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
            //m_maxTextSize = 56.0f;
            removeComponent = true;
        }

        ImGui::EndPopup();
    }

    // Draw component specific parameters ui.
    if (componentTreeOpen && !removeComponent)
    {
        drawParameters(pComponent);
    }

    ImGui::Separator();
    ImGui::PopID();
}

template<typename T>
void AddComponentMenuItem(const char *label)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    static_assert(requires{ pData->m_selectedEntity.TryGetComponents<T>(); });

    if (ImGui::MenuItem(label))
    {
        if (!pData->m_selectedEntity.TryGetComponents<T>())
        {
            pData->m_selectedEntity.AddComponent<T>();
        }
        else
        {
            SL_LOG_WARN("Entity {} already has component {}", (uint32_t)pData->m_selectedEntity.GetHandle(), nameof::nameof_type<T>());
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

        std::string &name = pComponent->m_name;
        constexpr size_t BufferSize = 64;
        SL_ASSERT(BufferSize > name.size(),
            "ImGui ensure that InputText() returns a null-terminated character array,"
            "it also means that character buffer[BufferSize - 1] will be discard.");
        char buffer[BufferSize] = { 0 };
        memcpy(buffer, name.data(), name.size());

        StartWithText("Name");
        if (ImGui::InputText("##Name", buffer, BufferSize))
        {
            name = buffer;
            if (name.empty())
            {
                name = "New Entity";
            }
        }
    });

    // Draw transform component
    DrawComponent<sl::TransformComponent>("Transform", [this, pData](sl::TransformComponent *pComponent)
    {
        bool cameraMayBeDirty = false;

        glm::vec3 &position = pComponent->m_position;
        StartWithText("Position");
        if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.1f))
        {
            cameraMayBeDirty = true;
        }

        glm::vec3 ratationDegrees = RotationModAndRepeat(pComponent->GetRotationDegrees());
        StartWithText("Rotation");
        if (ImGui::DragFloat3("##Rotation", glm::value_ptr(ratationDegrees), 0.1f))
        {
            pComponent->SetRotationDegrees(ratationDegrees);
            cameraMayBeDirty = true;
        }

        glm::vec3 &scale = pComponent->m_scale;
        StartWithText("Scale");
        ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);

        if (cameraMayBeDirty)
        {
            // If we select a camera entitiy
            if (auto *pCamera = pData->m_selectedEntity.TryGetComponents<sl::CameraComponent>(); pCamera)
            {
                pCamera->m_position = pComponent->m_position;
                pCamera->m_rotation = pComponent->m_rotation;
                pCamera->m_isDirty = true;
            }
        }
    });

    // Draw Camera component
    DrawComponent<sl::CameraComponent>("Camera", [this, pData](sl::CameraComponent *pComponent)
    {
        StartWithText("Main Camera");
        bool isMainCamera = pComponent->m_isMainCamera;
        if (ImGui::Checkbox("##MainCameraCheckBox", &isMainCamera))
        {
            if (isMainCamera)
            {
                auto view = sl::World::GetRegistry().group<sl::CameraComponent>();
                for (auto entity : view)
                {
                    view.get<sl::CameraComponent>(entity).m_isMainCamera = false;
                }
                pComponent->m_isMainCamera = true;
            }
            else
            {
                SL_LOG_WARN("Main camera must exist in the scene!");
            }
        }

        constexpr size_t ProjectionTypeCount = nameof::enum_count<sl::ProjectionType>();
        constexpr std::array<const char *, ProjectionTypeCount> ProjectionTypeNames =
        {
            nameof::nameof_enum(sl::ProjectionType::Perspective).data(),
            nameof::nameof_enum(sl::ProjectionType::Orthographic).data(),
        };

        StartWithText("Projection");
        if (ImGui::BeginCombo("##Projection", ProjectionTypeNames[(size_t)pComponent->m_projectionType], ImGuiComboFlags_WidthFitPreview))
        {
            for (size_t i = 0; i < ProjectionTypeCount; ++i)
            {
                if (ImGui::Selectable(ProjectionTypeNames[i], i == (size_t)pComponent->m_projectionType))
                {
                    pComponent->m_projectionType = (sl::ProjectionType)i;
                    pComponent->m_isDirty = true;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Perspective", DefaultSubTreeFlags, "Perspective"))
        {
            ImGui::Indent();

            float fovDegrees = glm::degrees(pComponent->m_fov);
            StartWithText("FOV");
            if (ImGui::DragFloat("##FOV", &fovDegrees, 0.1f, 1.0f, 120.0f))
            {
                pComponent->m_fov = glm::radians(fovDegrees);
                pComponent->m_isDirty = true;
            }

            StartWithText("Near Plane");
            if (ImGui::DragFloat("##NearPlane", &(pComponent->m_nearPlane), 0.001f, 0.01f, 1.0f))
            {
                pComponent->m_isDirty = true;
            }

            StartWithText("Far Plane");
            if (ImGui::DragFloat("##FarPlane", &(pComponent->m_farPlane), 100.0f, 1.0f, 100000.0f))
            {
                pComponent->m_isDirty = true;
            }

            ImGui::Unindent();
        }
        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Orthographic", DefaultSubTreeFlags, "Orthographic"))
        {
            ImGui::Indent();

            StartWithText("Size");
            if (ImGui::DragFloat("##Size", &(pComponent->m_orthoSize), 100.0f, 1.0f, 100000.0f))
            {
                pComponent->m_isDirty = true;
            }

            StartWithText("Near Clip");
            if (ImGui::DragFloat("##NearClip", &(pComponent->m_orthoNearClip), 100.0f, -100000.0f, 100000.0f))
            {
                pComponent->m_isDirty = true;
            }

            StartWithText("Far Clip");
            if (ImGui::DragFloat("##FarClip", &(pComponent->m_orthoFarClip), 100.0f, -100000.0f, 100000.0f))
            {
                pComponent->m_isDirty = true;
            }

            ImGui::Unindent();
        }
        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Controller", DefaultSubTreeFlags, "Controller"))
        {
            ImGui::Indent();

            float rotateSpeedDegrees = glm::degrees(pComponent->m_rotateSpeed);
            StartWithText("Rotate Speed");
            if (ImGui::DragFloat("##RotateSpeed", &rotateSpeedDegrees, 0.001f, 0.001f, 0.5f))
            {
                pComponent->m_rotateSpeed = glm::radians(rotateSpeedDegrees);
            }

            StartWithText("Shift Multiplier");
            ImGui::DragFloat("##ShiftMultiplier", &(pComponent->m_moveSpeedKeyShiftMultiplier), 0.01f, 0.1f, 10.0f);

            StartWithText("Scroll Multiplier");
            ImGui::DragFloat("##ScrollMultiplier", &(pComponent->m_moveSpeedMouseScrollMultiplier), 0.01f, 0.1f, 10.0f);

            ImGui::Unindent();
        }
    });

    // Add component button
    AlignNextWidget("Add Component");
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponentPopup");
    }
    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        AddComponentMenuItem<sl::CameraComponent>("Camera");
        ImGui::EndPopup();
    }

    ImGui::End();
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

float Details::StartWithText(std::string_view text, float offset)
{
    static sl::Entity s_crtEntity;
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (s_crtEntity != pData->m_selectedEntity)
    {
        /*
         * `ImGui::CalcTextSize("Rotation").x == 56.0f`
         * `ImGui::CalcTextSize("Position").x == 56.0f`
         * Just a little trick to avoid tag component flickering when it is rendered the first time,
         * as we known every entity must hold both tag and transform component.
         */
        m_maxTextSize = 56.0f;
        s_crtEntity = pData->m_selectedEntity;
    }

    float crtTextSize = ImGui::CalcTextSize(text.data()).x;
    float crtOffset;
    if (offset)
    {
        crtOffset = offset;
    }
    else
    {
        m_maxTextSize = std::max(m_maxTextSize, crtTextSize);
        crtOffset = ImGui::GetStyle().IndentSpacing;
    }

    ImGui::SetCursorPosX(crtOffset);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(text.data());

    float padding = ImGui::GetStyle().WindowPadding.x;
    float nextOffset = crtOffset + m_maxTextSize + padding;
    ImGui::SameLine(nextOffset);
    ImGui::SetNextItemWidth(-padding);

    return nextOffset;
}
