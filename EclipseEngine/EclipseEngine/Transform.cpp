
#define GLM_ENABLE_EXPERIMENTAL

#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform()
    : position(0.0f), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(1.0f) {}

glm::mat4 Transform::GetMatrix() const
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotation); // Convert quaternion to matrix
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::GetWorldMatrix(const glm::mat4& parentMatrix) const
{
    glm::mat4 localTransform = GetMatrix();
    return parentMatrix * localTransform;
}

void Transform::SetMatrix(const glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    // Decompose the matrix
    bool success = glm::decompose(matrix, scale, rotation, position, skew, perspective);
    if (success)
    {
        // Normalize the quaternion to avoid skew
        rotation = glm::normalize(rotation);
    }
    else
    {
        // Handle failure (e.g., reset to defaults)
        position = glm::vec3(0.0f);
        rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        scale = glm::vec3(1.0f);
    }
}

void Transform::SetPosition(const glm::vec3& newPosition)
{
    position = newPosition;
}

void Transform::SetRotation(const glm::quat& newRotation)
{
    rotation = glm::normalize(newRotation);
}

void Transform::SetRotationFromEuler(const glm::vec3& eulerAngles)
{
    rotation = glm::normalize(glm::quat(glm::radians(eulerAngles)));
}

void Transform::SetScale(const glm::vec3& newScale)
{
    scale = glm::max(newScale, glm::vec3(0.0001f)); // Prevent zero or negative scales
}

void Transform::SetFromMatrix(const glm::mat4& matrix) {
    position = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
    rotation = glm::quat_cast(matrix);
    scale = glm::vec3(glm::length(glm::vec3(matrix[0])),
        glm::length(glm::vec3(matrix[1])),
        glm::length(glm::vec3(matrix[2])));
}

glm::vec3 Transform::GetEulerAngles() const
{
    return glm::degrees(glm::eulerAngles(rotation));
}

void Transform::Update()
{
    // If you need to do any updates before rendering, do it here
}
