#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

class Transform : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();

    glm::mat4 GetMatrix() const;
    void Update() override;
};

#endif // TRANSFORM_H
