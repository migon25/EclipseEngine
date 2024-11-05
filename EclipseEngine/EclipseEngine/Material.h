#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "Component.h"

class Material : public Component 
{
public:
    Material(Shader& shader, const std::vector<Texture>& textures);

    void BindTextures();
    void Update() override;

public:
    Shader& shader;
    std::vector<Texture> textures;

};

#endif // MATERIAL_H