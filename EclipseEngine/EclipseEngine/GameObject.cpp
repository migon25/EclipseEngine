#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
    // Initialization code for the GameObject
}

GameObject::~GameObject()
{
    // placeholder for Cleanup
}

void GameObject::Update()
{
    // placeholder for Update
}

void GameObject::Draw(Shader& shader, Camera& camera, const glm::mat4& parentTransform)
{

    // Get the model matrix from the Transform component
    glm::mat4 objectModel = transform.GetWorldMatrix(parentTransform); // Get the transformation matrix from Transform


    // Activate the shader program
    if(!material)
    { 
        std::vector<Texture> defaultTex{ Texture("Assets/Textures/default.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
        this->AddComponent<Material>(defaultTex); // Add a default material if none exists
    }

    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform1f(glGetUniformLocation(shader.ID, "outlining"), 1.08f);

    if(material) material.get()->BindTextures(shader); // If it has material, bind it

    // Code to draw the game object (calls Draw on the mesh if it exists)
    if (mesh)
    {
        mesh->Draw(shader, camera);
    }

    for (const auto& child : children) {
        child->Draw(shader, camera, objectModel);
    }
}