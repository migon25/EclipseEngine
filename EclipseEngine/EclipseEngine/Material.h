#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "Component.h"

class Material : public Component {
public:
    Shader* shader;
    std::vector<Texture> textures;

    Material(Shader* shader, const std::vector<Texture>& textures);

    void BindTextures() const;
    void Update() override;
};

#endif // MATERIAL_H