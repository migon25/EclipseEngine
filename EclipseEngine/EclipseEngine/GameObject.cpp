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
   
    if(material) material.get()->BindTextures(); // If it has material, bind it

    // Code to draw the game object (calls Draw on the mesh if it exists)
    if (mesh) {
        mesh->Draw(shader, camera);
    }
}