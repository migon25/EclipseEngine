#include "ModelLoader.h"
#include <iostream>
#include <set>
#include "Logger.h"

std::shared_ptr<GameObject> ModelLoader::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return std::shared_ptr<GameObject>(); // Return an empty GameObject
    }

    auto model = std::make_shared<GameObject>();
    ProcessNode(scene->mRootNode, scene, model);
    return std::move(model);
}

std::shared_ptr<GameObject> ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<GameObject>& parent) {
    // Create a new GameObject for the current node
    std::cout << "Processing Node: " << node->mName.C_Str() << std::endl;
    std::shared_ptr<GameObject> currentNode = std::make_shared<GameObject>();
    currentNode->name = node->mName.C_Str();

    // Process all meshes for this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        ProcessMesh(mesh, scene, vertices, indices, textures);
        std::cout << "Mesh has " << vertices.size() << " vertices and " << indices.size() << " indices.\n";
        // Add a Mesh component to the current node
        currentNode->AddComponent<Mesh>(vertices, indices, textures);
    }

    // Add the current node as a child of the parent GameObject
    parent->AddChild(currentNode);

    // Recursively process children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, currentNode);
    }

	return currentNode;
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene,
    std::vector<Vertex>& vertices,
    std::vector<GLuint>& indices,
    std::vector<Texture>& textures) {
    // Process vertex positions, normals, and texture coordinates
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

    // Process textures
    //if (mesh->mMaterialIndex >= 0) {
    //    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    //    textures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    //}
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    std::vector<Texture> textures;
    std::set<std::string> loadedTextures; // To track loaded texture paths

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str; // For texture file path
        mat->GetTexture(type, i, &str);

        // Check if texture is already loaded
        if (loadedTextures.find(str.C_Str()) != loadedTextures.end())
        {
            continue; // Skip if already loaded
        }

        // Load the texture and add it to the vector
        Texture texture(str.C_Str(), typeName, textures.size(), GL_RGBA, GL_UNSIGNED_BYTE);
        textures.push_back(texture);
        loadedTextures.insert(str.C_Str()); // Track the loaded texture path
    }
    return textures;
}

