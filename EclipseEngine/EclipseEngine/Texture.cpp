#include "Texture.h"
#include <iostream>
#include "Logger.h"

Texture::Texture(const char* filename, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
    : type(texType), unit(slot)
{
    // Assigns the type of the texture to the texture object
    type = texType;

    // Generate and bind a DevIL image ID
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load the image using DevIL
    if (!ilLoadImage((const wchar_t*)filename)) { // Use filename directly here
        Logger::Log("Failed to load image");
        std::cerr << "Failed to load image: " << filename << std::endl;
        ilDeleteImages(1, &imageID);
        return;
    }
    Logger::Log("Image loaded");

    // Convert the image to RGBA format
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Generates an OpenGL texture object
    glGenTextures(1, &textureID);
    // Assigns the texture to a Texture Unit
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Assigns the image to the OpenGL Texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
        0, format, pixelType, ilGetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generates MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up the DevIL image ID
    ilDeleteImages(1, &imageID);

    // Unbinds the OpenGL Texture object so that it can't accidentally be modified
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Constructor: Initializes DevIL and loads a texture from file
Texture::Texture(const aiString& filename, const std::string& texType, GLuint slot, GLenum format, GLenum pixelType)
    : type(texType), unit(slot)
{
    // Generate and bind a DevIL image ID
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load the image using DevIL
    if (!ilLoadImage((const wchar_t*)filename.C_Str())) {
        Logger::Log("Failed to load image");
        std::cerr << "Failed to load image: " << (const wchar_t*)filename.C_Str() << std::endl;
        ilDeleteImages(1, &imageID);
        return;
    }
    Logger::Log("Image loaded");

    // Convert the image to RGBA format
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Generates an OpenGL texture object
    glGenTextures(1, &textureID);
    // Assigns the texture to a Texture Unit
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Assigns the image to the OpenGL Texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
        0, format, pixelType, ilGetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generates MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clean up the DevIL image ID (correcting the cleanup)
    ilDeleteImages(1, &imageID); // <- Correct cleanup

    // Unbind the OpenGL Texture object to prevent accidental modifications
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit)
{
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    // Shader needs to be activated before changing the value of a uniform
    shader.Activate();
    // Sets the value of the uniform
    glUniform1i(texUni, unit);
}

void Texture::Bind() const // Ensure this is const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &textureID);
}