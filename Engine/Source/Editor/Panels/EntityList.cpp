#include "EntityList.h"

#include "Panels/ImGuiData.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"

#include <imgui/imgui.h>

void EntityList::OnAttach()
{

}

void EntityList::OnDetach()
{

}

void EntityList::BeginFrame()
{

}

void EntityList::OnUpdate(float deltaTime)
{
    SL_PROFILE;

    if (!ImGui::Begin("Entity List"))
    {
        ImGui::End();
        return;
    }

    bool isEntityClicked = false;
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    // Each entity holds a #TagComponent
    auto view = sl::World::GetRegistry().view<sl::TagComponent>();
    for (auto entity : view)
    {
        // TODO: Hierarchy
        ImGuiTreeNodeFlags treeNodeFlag =
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen |
            ImGuiTreeNodeFlags_FramePadding |
            ImGuiTreeNodeFlags_SpanAvailWidth;
        if (pData->m_selectedEntity == entity)
        {
            treeNodeFlag |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::PushID((uint32_t)entity);
        ImGui::TreeNodeEx(view.get<sl::TagComponent>(entity).m_name.data(), treeNodeFlag);

        // Left click to select entity
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            isEntityClicked = true;
            pData->m_selectedEntity = entity;
        }
        // Right click to open an entity popup
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            isEntityClicked = true;
            pData->m_selectedEntity = entity;
            ImGui::OpenPopup("##EntityPopup");
        }
        if (ImGui::BeginPopup("##EntityPopup"))
        {
            if (ImGui::MenuItem("Destory Entity"))
            {
                sl::Entity slEntity{ entity };

                // Not allowed to destroy the main camera
                auto pCameraComponent = slEntity.TryGetComponents<sl::CameraComponent>();
                if (pCameraComponent && pCameraComponent->m_isMainCamera)
                {
                    SL_LOG_WARN("Can not destroy main camera entity {}", (uint32_t)entity);
                }
                else
                {
                    if (pData->m_selectedEntity == slEntity)
                    {
                        pData->m_selectedEntity.ResetHandle();
                    }
                    slEntity.Destroy();
                }
            }
            ImGui::EndPopup();
        }
        ImGui::PopID();
    }

    if (ImGui::IsWindowHovered() && !isEntityClicked)
    {
        // Left click on enpty space to clear selected entity
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            pData->m_selectedEntity.ResetHandle();
        }
        // Right click on enpty space to create a new entity
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("##CreateNewEntityPopup");
        }
    }
    if (ImGui::BeginPopup("##CreateNewEntityPopup"))
    {
        if (ImGui::MenuItem("Creat New Entity"))
        {
            pData->m_selectedEntity = sl::World::CreateEntity();
        }
        if (ImGui::MenuItem("Creat Directional Light"))
        {
            auto entity = sl::World::CreateEntity("Directional Light");
            auto light = entity.AddComponent<sl::LightComponent>();
            light.m_type = sl::LightType::Directional;

            pData->m_selectedEntity = entity;
        }
        if (ImGui::MenuItem("Creat Point Light"))
        {
            auto entity = sl::World::CreateEntity("Point Light");
            auto light = entity.AddComponent<sl::LightComponent>();
            light.m_type = sl::LightType::Point;

            pData->m_selectedEntity = entity;
        }
        if (ImGui::MenuItem("Creat Spot Light"))
        {
            auto entity = sl::World::CreateEntity("Spot Light");
            auto light = entity.AddComponent<sl::LightComponent>();
            light.m_type = sl::LightType::Spot;

            pData->m_selectedEntity = entity;
        }
        ImGui::EndPopup();
    }
    ImGui::End(); // Entity List
}

void EntityList::OnRender()
{

}

void EntityList::EndFrame()
{

}

void EntityList::OnEvent(sl::Event &event)
{

}
