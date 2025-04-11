#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sl
{

struct TransformComponent
{
    void Reset();

    void SetRotationDegrees(const glm::vec3 &degrees);
    glm::vec3 GetRotationDegrees() const;

    glm::mat4 GetTranslate() const;
    glm::mat4 GetRotate() const;
    glm::mat4 GetScale() const;
    glm::mat4 GetTransform() const;

    glm::vec3 m_position{ 0.0f, 0.0f , 0.0f };
    glm::vec3 m_rotation{ 0.0f, 0.0f , 0.0f }; // Stores in radians
    glm::vec3 m_scale{ 1.0f, 1.0f , 1.0f };
};

} // namespace sl
