#pragma once

#include "Shader/Shared.h"

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace sl
{

enum class LightType : uint8_t
{
    Directional = SL_TYPE_LIGHT_DIRECTIONAL,
    Point       = SL_TYPE_LIGHT_POINT,
    Spot        = SL_TYPE_LIGHT_SPOT,
};

struct LightComponent
{
    void Reset()
    {
        m_range = 128.0f;
        m_intensity = 1024.0f;
        m_outer = glm::radians(45.0f);
        m_inner = glm::radians(30.0f);
        m_color = glm::vec3{ 1.0f };
    }

    float m_intensity = 1024.0f;
    float m_range = 128.0f;
    float m_outer = glm::radians(45.0f); // Stores in radiance
    float m_inner = glm::radians(30.0f); // Stores in radiance
    glm::vec3 m_color{ 1.0f };
    LightType m_type = LightType::Point;
};

} // namespace sl
