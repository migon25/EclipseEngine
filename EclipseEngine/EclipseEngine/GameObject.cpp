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

// Compute world transform
glm::mat4 GameObject::CalculateWorldTransform(const glm::mat4& parentTransform) const {

	if (parent)
	{
		return parentTransform * transform.GetMatrix(); // Combine parent's and local transform
	}
    return transform.GetMatrix();
}

void GameObject::UpdateChildrenTransforms()
{
	for (const auto& child : children) {
		child->transform.SetMatrix(CalculateWorldTransform(child->parent->transform.GetMatrix()));
		child->UpdateChildrenTransforms();
	}
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
	shader.SetMat4("model", objectModel);
    shader.SetFloat("outlining", 1.05f);

    if(material) material.get()->BindTextures(shader); // If it has material, bind it
    if (mesh) mesh->Draw(shader, camera);   // Code to draw the game object (calls Draw on the mesh if it exists)

    for (const auto& child : children) {
        child->Draw(shader, camera, objectModel);
    }
}