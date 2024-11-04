#include "Material.h"

Material::Material(Shader& shader, const std::vector<Texture>& textures)
    : shader(shader), textures(textures) {}

void Material::BindTextures()  {

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); ++i) {

		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
        else {
            std::cerr << "Texture " << i << " not valid." << std::endl;
        }
		textures[i].TexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
    }
}

void Material::Update() {
    // Any update logic specific to Material can go here
}
