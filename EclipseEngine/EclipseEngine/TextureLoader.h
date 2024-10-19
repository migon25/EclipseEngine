#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GL/glew.h>  // For OpenGL types
#include <IL/il.h>    // For DevIL

class TextureLoader {
public:
    TextureLoader();  // Constructor to initialize DevIL
    ~TextureLoader(); // Destructor (optional, for future needs)

    GLuint loadTexture(const char* filename); // Load texture from file

private:
    void initializeDevIL(); // Initialize DevIL (called internally)
};

#endif // TEXTURE_LOADER_H