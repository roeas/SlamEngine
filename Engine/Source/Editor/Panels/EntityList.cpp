#include "EntityList.h"

#include "Panels/ImGuiData.h"
#include "Scene/World.h"

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
    if (!ImGui::Begin("Entity List"))
    {
        ImGui::End();
        return;
    }
    
    bool isEntityClicked = false;
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    // Each entity holds a #TagComponent
    auto allEntityView = sl::World::GetRegistry().view<sl::TagComponent>();
    for (auto entity : allEntityView)
    {
        ImGuiTreeNodeFlags treeNodeFlag =
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_FramePadding |
            ImGuiTreeNodeFlags_SpanAvailWidth;
        if (pData->m_selectedEntity == entity)
        {
            treeNodeFlag |= ImGuiTreeNodeFlags_Selected;
        }

        bool nodeOpen = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity,
            treeNodeFlag, allEntityView.get<sl::TagComponent>(entity).m_name.data());
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
            ImGui::OpenPopup("##EntityPopup");
        }
        if (ImGui::BeginPopup("##EntityPopup"))
        {
            if (ImGui::MenuItem("Destory Entity"))
            {
                sl::Entity slEntity{ entity };

                // Not allowed to destroy the main camera
                if (auto pCameraComponent = slEntity.TryGetComponents<sl::CameraComponent>();
                    pCameraComponent && pCameraComponent->m_isMainCamera)
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

        if (nodeOpen)
        {
            // TODO: Hierarchy
            ImGui::TreePop();
        }
    }

    if (ImGui::IsWindowHovered() && !isEntityClicked)
    {
        // Left click on an enpty space to clear selected entity
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            pData->m_selectedEntity.ResetHandle();
        }
        // Right click on an enpty space to create a new entity
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
        ImGui::EndPopup();
    }
    ImGui::End();
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
