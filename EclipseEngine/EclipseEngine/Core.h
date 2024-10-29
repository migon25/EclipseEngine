#ifndef CORE_H
#define CORE_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class Core {
public:
    Core(int width, int height, const std::string& title);
    ~Core();

    bool Initialize();
    void BeginFrame();
    void EndFrame();
    bool ShouldClose() const;
    GLFWwindow* GetWindow() const;

private:
    GLFWwindow* m_Window;
    int m_Width, m_Height;
    std::string m_Title;

    bool InitializeGLFW();
    bool InitializeOpenGL();
};


#endif // CORE_H