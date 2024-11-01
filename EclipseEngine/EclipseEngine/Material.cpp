#include "Material.h"

Material::Material(Shader& shader, const std::vector<Texture>& textures)
    : shader(shader), textures(textures) {}

void Material::BindTextures()  {
    for (unsigned int i = 0; i < textures.size(); ++i) {

        std::string num;
        std::string type = textures[i].type;
        // Ensure texture is available and correctly assigned in the shader
        if (textures[i].textureID != 0) { // Check if the textureID is valid
            textures[i].TexUnit(shader, ("diffuse" + std::to_string(i)).c_str(), i);
            textures[i].Bind();
        }
        else {
            std::cerr << "Texture " << i << " not valid." << std::endl;
        }
    }
}

void Material::Update() {
    // Any update logic specific to Material can go here
}
