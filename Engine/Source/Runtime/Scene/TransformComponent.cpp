#include "TransformComponent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/trigonometric.hpp>

namespace sl
{

void TransformComponent::SetRotationDegrees(const glm::vec3 &degrees)
{
    m_rotation = glm::radians(degrees);
}

glm::vec3 TransformComponent::GetRotationDegrees() const
{
    return glm::degrees(m_rotation);
}

glm::mat4 TransformComponent::GetTranslate() const
{
    return glm::translate(glm::mat4{ 1.0f }, m_position);
}

glm::mat4 TransformComponent::GetRotate() const
{
    return glm::toMat4(glm::quat(m_rotation));
}

glm::mat4 TransformComponent::GetScale() const
{
    return glm::scale(glm::mat4{ 1.0f }, m_scale);
}

glm::mat4 TransformComponent::GetTransform() const
{
    return glm::translate(glm::mat4{ 1.0f }, m_position) *
        glm::toMat4(glm::quat(m_rotation)) *
        glm::scale(glm::mat4{ 1.0f }, m_scale);
}

} // namespace sl
