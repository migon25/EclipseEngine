#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>  
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>    
#include "Shader.h"
#include <string>  // Include string to use std::string

class Texture {
public:
	ILuint imageID = 0;
	GLuint textureID = 0;
	std::string type;
	GLuint unit;
	std::string path;

	Texture(const char* filename, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	Texture(const std::string filename, const std::string& texType, GLuint slot, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);

	// Assigns a texture unit to a texture
	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind() const;
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};

#endif // TEXTURE_H
