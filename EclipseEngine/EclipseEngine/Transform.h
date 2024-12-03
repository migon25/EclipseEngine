#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

class Transform : public Component 
{
public:
    Transform();

    glm::mat4 GetMatrix() const;
	void SetMatrix(const glm::mat4& matrix);

    void SetPosition(const glm::vec3& newPosition);
    void SetRotation(const glm::vec3& newRotation);
    void SetScale(const glm::vec3& newScale);

    void Update() override;

public:
    glm::vec3 position;
    glm::vec3 rotation; // In degrees, maybe change it in future updates? idk yet
    glm::vec3 scale;
};

#endif // TRANSFORM_H
