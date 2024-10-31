#include "Transform.h"

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation around X axis
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation around Y axis
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation around Z axis

    // Combine the rotation matrices
    glm::mat4 rot = rotZ * rotY * rotX; // Order of rotation can be adjusted

    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
    return trans * rot * scl;
}

void Transform::Update() {
    // Any update logic specific to Transform can go here
}
