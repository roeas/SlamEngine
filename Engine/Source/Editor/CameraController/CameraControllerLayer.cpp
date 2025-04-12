#include "CameraControllerLayer.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Scene/World.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <algorithm>
#include <array>

void CameraControllerLayer::OnAttach()
{

}

void CameraControllerLayer::OnDetach()
{

}

void CameraControllerLayer::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<sl::MouseScrollEvent>(SL_BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseScroll));
}

void CameraControllerLayer::BeginFrame()
{

}

void CameraControllerLayer::OnUpdate(float deltaTime)
{
    auto mode = sl::World::GetMainCameraComponent().m_controllerMode;
    if (mode == sl::CameraControllerMode::None)
    {
        return;
    }

    if (mode == sl::CameraControllerMode::FPS)
    {
        UpdateFPSMode(deltaTime);
    }
    else if (mode == sl::CameraControllerMode::Editor)
    {
        UpdateEditorMode(deltaTime);
    }
}

void CameraControllerLayer::OnRender()
{

}

void CameraControllerLayer::EndFrame()
{

}

void CameraControllerLayer::UpdateFPSMode(float deltaTime)
{
    auto [camera, transform] = sl::World::GetMainCameraEntity().GetComponents<sl::CameraComponent, sl::TransformComponent>();

    // Rotation
    {
        glm::vec2 offset = sl::Input::GetMouseDelta();
        transform.m_rotation += glm::vec3{ glm::vec2{ offset.x , -offset.y } * camera.m_rotateSpeed, 0.0f };
        transform.m_rotation.y = std::clamp(transform.m_rotation.y, glm::radians(-89.9f), glm::radians(89.9f));

        camera.m_isDirty = true;
    }

    // Movement
    constexpr std::array<uint32_t, 6> CamraMoveKey =
    {
        SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D, SL_KEY_Q, SL_KEY_E,
    };

    uint8_t moveKeyMask = 0x00;
    for (size_t i = 0; i < CamraMoveKey.size(); ++i)
    {
        if (sl::Input::IsKeyDown(CamraMoveKey[i]))
        {
            moveKeyMask |= UINT8_C(1 << i);
        }
    }

    glm::vec3 finalMoveDir{ 0.0f };
    if (moveKeyMask)
    {
        const std::array<glm::vec3, 6> moveBehaviors =
        {
            camera.m_frontDir,  // SL_KEY_W
            -camera.m_rightDir, // SL_KEY_A
            -camera.m_frontDir, // SL_KEY_S
            camera.m_rightDir,  // SL_KEY_D
            -camera.m_upDir,    // SL_KEY_Q
            camera.m_upDir,     // SL_KEY_E
        };
        for (size_t i = 0; i < CamraMoveKey.size(); ++i)
        {
            if (moveKeyMask & UINT8_C(1 << i))
            {
                finalMoveDir += moveBehaviors[i];
            }
        }
        finalMoveDir = glm::normalize(finalMoveDir);

        if (glm::any(glm::isnan(finalMoveDir)))
        {
            finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
        }
    }

    float moveSpeedKeyShiftMultiplier = sl::Input::IsKeyDown(SL_KEY_LSHIFT) ? camera.m_moveSpeedKeyShiftMultiplier : 1.0f;
    float finalMoveSpeed = camera.m_moveSpeed * moveSpeedKeyShiftMultiplier * camera.m_moveSpeedMouseScrollMultiplier;

    transform.m_position += finalMoveDir * finalMoveSpeed * deltaTime;
    camera.m_position = transform.m_position;
    camera.m_rotation = transform.m_rotation;
    camera.m_isDirty = true;

    SL_LOG_DEBUG("Pos: {}", camera.m_position);
}

void CameraControllerLayer::UpdateEditorMode(float deltaTime)
{
     // TODO
}

bool CameraControllerLayer::OnMouseScroll(sl::MouseScrollEvent &event)
{
    auto &camera = sl::World::GetMainCameraComponent();
    if (camera.m_controllerMode == sl::CameraControllerMode::None)
    {
        return false;
    }

    camera.m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.1f;
    camera.m_moveSpeedMouseScrollMultiplier = std::clamp(camera.m_moveSpeedMouseScrollMultiplier, 0.1f, 10.0f);

    return true;
}

