#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "Component.h" 
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include "ModelLoader.h"

class Mesh : public Component {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO VAO;

    // Constructor for creating a mesh with vertices, indices, and textures
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    // Constructor for loading a mesh from a file path
    Mesh(const std::string& filepath);

    std::vector<Texture>& GetTextures() {
        return textures; // Return a reference to the textures vector
    }

    void Update() override; // Provide an empty Update if necessary
    void Draw(Shader& shader, Camera& camera);
};

#endif // MESH_H