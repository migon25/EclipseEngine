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

void GameObject::Draw(Camera& camera) 
{

    // Get the model matrix from the Transform component
    glm::mat4 objectModel = transform.GetMatrix(); // Get the transformation matrix from Transform

    // Activate the shader program
    if(!material)
    { 
        std::vector<Texture> defaultTex{ Texture("Assets/Textures/default.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE) };
        this->AddComponent<Material>(defaultTex); // Add a default material if none exists
    }

    material.get()->defaultShader->Activate();
    glUniformMatrix4fv(glGetUniformLocation(material.get()->defaultShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform1f(glGetUniformLocation(material.get()->defaultShader->ID, "outlining"), 1.08f);

    if(material) material.get()->BindTextures(); // If it has material, bind it

    // Code to draw the game object (calls Draw on the mesh if it exists)
    if (mesh)
    {
        mesh->Draw(*material.get()->defaultShader, camera);
    }
}