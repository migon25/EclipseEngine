#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>  
#include <IL/il.h>    
#include "Shader.h"

class Texture {
public:
	ILuint imageID = 0;
    GLuint textureID = 0;
    GLenum type;

    Texture(const char* filename, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};

#endif // TEXTURE_H
