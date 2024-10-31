#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>
#include <vector>
#include "VBO.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLoader {
public:
    // Static method to load a model from a file path
    static void LoadModel(const std::string& filepath,
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices,
        std::vector<Texture>& textures);

private:
    static void ProcessNode(aiNode* node, const aiScene* scene,
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices,
        std::vector<Texture>& textures);

    static void ProcessMesh(aiMesh* mesh, const aiScene* scene,
        std::vector<Vertex>& vertices,
        std::vector<GLuint>& indices,
        std::vector<Texture>& textures);

    // std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

#endif // MODELLOADER_H