#include "GameObject.h"
#include <iostream>

GameObject::GameObject() {
    // Initialization code for the GameObject
}

GameObject::~GameObject() {
    // placeholder for Cleanup
}

void GameObject::Update() {
    // Code to update the game object's state, e.g., update transform, physics, etc.
    std::cout << "Updating GameObject" << std::endl;
}

void GameObject::Draw(Shader& shader, Camera& camera) {
    // Code to draw the game object (calls Draw on the mesh if it exists)
    if (mesh) {
        mesh->Draw(shader, camera);
    }
}