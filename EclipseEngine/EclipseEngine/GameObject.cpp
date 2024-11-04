#include "GameObject.h"
#include <iostream>

GameObject::GameObject() {
    // Initialization code for the GameObject
}

GameObject::~GameObject() {
    // placeholder for Cleanup
}

void GameObject::Update() {
}

void GameObject::Draw(Shader& shader, Camera& camera) {
    // Get the model matrix from the Transform component
    glm::mat4 objectModel = transform.GetMatrix(); // Get the transformation matrix from Transform

    // Activate the shader program
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
   
    if(material) material.get()->BindTextures(); // If it has material, bind it

    // Code to draw the game object (calls Draw on the mesh if it exists)
    if (mesh) {
        mesh->Draw(shader, camera);
    }
}