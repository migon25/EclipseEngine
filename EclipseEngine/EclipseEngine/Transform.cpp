#include "Transform.h"

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z
    model = glm::scale(model, scale);
    return model;
}

void Transform::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Transform::SetRotation(const glm::vec3& newRotation) {
    rotation = newRotation;
}

void Transform::SetScale(const glm::vec3& newScale) {
    scale = newScale;
}

void Transform::Update() {
    // If you need to do any updates before rendering, do it here
}
