#include "CameraControllerLayer.h"

#include "Event/MouseEvent.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"
#include "Window/Input.h"

#include <array>

void CameraControllerLayer::OnAttach()
{

}

void CameraControllerLayer::OnDetach()
{

}

void CameraControllerLayer::BeginFrame()
{

}

void CameraControllerLayer::OnUpdate(float deltaTime)
{
    SL_PROFILE;

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

void CameraControllerLayer::OnEvent(sl::Event &event)
{
    SL_PROFILE;

    sl::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<sl::MouseScrollEvent>(SL_BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseScroll));
}

void CameraControllerLayer::UpdateFPSMode(float deltaTime)
{
    auto [camera, transform] = sl::World::GetMainCameraEntity().GetComponents<sl::CameraComponent, sl::TransformComponent>();

    // Rotation
    {
        glm::vec2 offset = sl::Input::GetMouseDelta();
        transform.m_rotation += glm::vec3{ glm::vec2{ -offset.y, offset.x } * camera.m_rotateSpeed, 0.0f };
        transform.m_rotation.x = std::clamp(transform.m_rotation.x, glm::radians(-89.9f), glm::radians(89.9f));
    }

    // Movement
    constexpr size_t MoveDirCount = 6;
    constexpr std::array<uint32_t, MoveDirCount> MoveKeys =
    {
        SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D, SL_KEY_Q, SL_KEY_E,
    };
    const std::array<glm::vec3, MoveDirCount> MoveDirs =
    {
        camera.m_frontDir,  // SL_KEY_W
        -camera.m_rightDir, // SL_KEY_A
        -camera.m_frontDir, // SL_KEY_S
        camera.m_rightDir,  // SL_KEY_D
        -camera.m_upDir,    // SL_KEY_Q
        camera.m_upDir,     // SL_KEY_E
    };

    glm::vec3 finalMoveDir{ 0.0f };
    for (size_t i = 0; i < MoveDirCount; ++i)
    {
        if (sl::Input::IsKeyDown(MoveKeys[i]))
        {
            finalMoveDir += MoveDirs[i];
        }
    }
    finalMoveDir = glm::normalize(finalMoveDir);
    if (glm::any(glm::isnan(finalMoveDir)))
    {
        finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
    }

    float moveSpeedKeyShiftMultiplier = sl::Input::IsKeyDown(SL_KEY_LSHIFT) ? camera.m_moveSpeedKeyShiftMultiplier : 1.0f;
    float finalMoveSpeed = camera.m_moveSpeed * moveSpeedKeyShiftMultiplier * camera.m_moveSpeedMouseScrollMultiplier;

    transform.m_position += finalMoveDir * finalMoveSpeed * deltaTime;
    camera.m_position = transform.m_position;
    camera.m_rotation = transform.m_rotation;
    camera.m_isDirty = true;
}

void CameraControllerLayer::UpdateEditorMode(float deltaTime)
{
     // TODO
}

bool CameraControllerLayer::OnMouseScroll(sl::MouseScrollEvent &event)
{
    SL_PROFILE;

    auto &camera = sl::World::GetMainCameraComponent();
    if (camera.m_controllerMode == sl::CameraControllerMode::None)
    {
        return false;
    }

    camera.m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.1f;
    camera.m_moveSpeedMouseScrollMultiplier = std::clamp(camera.m_moveSpeedMouseScrollMultiplier, 0.1f, 10.0f);

    return true;
}

