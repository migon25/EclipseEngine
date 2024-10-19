#include "TextureLoader.h"
#include <iostream>

// Constructor: Initializes DevIL
TextureLoader::TextureLoader() 
{
    initializeDevIL();
}

// Destructor: Clean up (optional)
TextureLoader::~TextureLoader() 
{
    // Currently, no specific cleanup needed
}

// Initialize DevIL
void TextureLoader::initializeDevIL() 
{
    ilInit();  // Initialize the DevIL library
    std::cout << "DevIL initialized.\n";
}

// Load texture from a file and return the OpenGL texture ID
GLuint TextureLoader::loadTexture(const char* filename)
{
    ILuint imageID;
    GLuint textureID;

    // Generate and bind a DevIL image ID
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Load the image using DevIL
    if (!ilLoadImage(filename))
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        ilDeleteImages(1, &imageID); // Clean up on failure
        return 0;
    }

    // Convert the image to RGBA format
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Generate an OpenGL texture ID and bind it
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Load the image data into OpenGL texture
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
        0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData()
    );

    // Set texture parameters (wrapping and filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Clean up the DevIL image ID
    ilDeleteImages(1, &imageID);

    return textureID; // Return the OpenGL texture ID
}
