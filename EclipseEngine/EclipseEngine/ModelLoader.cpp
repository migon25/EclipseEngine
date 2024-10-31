#include "ModelLoader.h"
#include <iostream>
#include "Logger.h"

void ModelLoader::LoadModel(const std::string& filepath,
    std::vector<Vertex>& vertices,
    std::vector<GLuint>& indices,
    std::vector<Texture>& textures) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    // Process Assimp's root node recursively
    ProcessNode(scene->mRootNode, scene, vertices, indices, textures);
    Logger::Log("model Loaded");
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene,
    std::vector<Vertex>& vertices,
    std::vector<GLuint>& indices,
    std::vector<Texture>& textures) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, vertices, indices, textures);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, vertices, indices, textures);
    }
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene,
    std::vector<Vertex>& vertices,
    std::vector<GLuint>& indices,
    std::vector<Texture>& textures) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }
        else {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Load textures
    //aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    //std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse"); // Fixed line
    //textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Texture> textures;
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);  // Get the texture path from the material
        std::string texturePath = str.C_Str();  // Convert aiString to std::string

        Texture texture(texturePath, typeName, i, GL_RGBA, GL_UNSIGNED_BYTE);  // Now create Texture
        textures.push_back(texture);
    }
    return textures;
}