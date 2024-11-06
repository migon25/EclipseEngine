#ifndef CORE_H
#define CORE_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "FrameBuffer.h"

class Core 
{
public:
    Core(int width, int height, const std::string& title, Framebuffer& framebuffer);
    ~Core();

    bool Initialize();
    void BeginFrame();
    void EndFrame();
    bool ShouldClose() const;
    GLFWwindow* GetWindow() const;

    Framebuffer& fbo;
private:
    GLFWwindow* m_Window;
    int m_Width, m_Height;
    std::string m_Title;

    bool InitializeGLFW();
    bool InitializeOpenGL();
};

#endif // CORE_H