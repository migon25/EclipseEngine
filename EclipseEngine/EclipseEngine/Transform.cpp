
#define GLM_ENABLE_EXPERIMENTAL

#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::GetMatrix() const 
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::yawPitchRoll(
        glm::radians(rotation.y),
        glm::radians(rotation.x),
        glm::radians(rotation.z));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::GetWorldMatrix(const glm::mat4& parentMatrix) const
{
    return parentMatrix * GetMatrix();
}

void Transform::SetMatrix(const glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    // Convert rotation (from quaternion to Euler angles)
    glm::quat rotationQuat = glm::quat_cast(matrix);
    rotation = glm::degrees(glm::eulerAngles(rotationQuat));

    // Decompose the matrix
    bool success = glm::decompose(matrix, scale, rotationQuat, position, skew, perspective);
    if (!success)
    {
        // Handle failure (e.g., reset to defaults)
        position = glm::vec3(0.0f);
        rotation = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        return;
    }
}

void Transform::SetPosition(const glm::vec3& newPosition) 
{
    position = newPosition;
}

void Transform::SetRotation(const glm::vec3& newRotation)
{
    rotation = newRotation;
}

void Transform::SetScale(const glm::vec3& newScale)
{
    scale = newScale;
}

void Transform::Update()
{
    // If you need to do any updates before rendering, do it here
}
