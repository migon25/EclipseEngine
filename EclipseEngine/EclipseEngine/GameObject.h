#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <memory>
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

class GameObject {
public:
    GameObject();
    ~GameObject();

    // Add a component to the game object
    template<typename T, typename... Args>
    void AddComponent(Args&&... args);

    // Retrieve a component of a specific type
    template<typename T>
    T* GetComponent();

    // Update and draw the game object
    void Update();
    void Draw(Shader& shader, Camera& camera);

    Transform transform;
private:
    std::unique_ptr<Material> material; // Using unique_ptr for automatic memory management
    std::unique_ptr<Mesh> mesh;         // Using unique_ptr for automatic memory management
};

// Template member function definitions should go here as well
template<typename T, typename... Args>
void GameObject::AddComponent(Args&&... args) {
    if constexpr (std::is_same<T, Mesh>::value) {
        mesh = std::make_unique<Mesh>(std::forward<Args>(args)...);
    }
    else if constexpr (std::is_same<T, Material>::value) {
        material = std::make_unique<Material>(std::forward<Args>(args)...);
    }
    // Add more components as needed
}

template<typename T>
T* GameObject::GetComponent() {
    if constexpr (std::is_same<T, Transform>::value) {
        return &transform; // Return the address of the Transform
    }
    else if constexpr (std::is_same<T, Material>::value) {
        return material.get(); // Return the raw pointer of the unique_ptr
    }
    else if constexpr (std::is_same<T, Mesh>::value) {
        return mesh.get(); // Return the raw pointer of the unique_ptr
    }
    return nullptr; // If no matching component, return nullptr
}

#endif // GAME_OBJECT_H
