#include <GL/glew.h>
#include "Core.h"
#include "Logger.h"

Core::Core(int width, int height, const std::string& title, Framebuffer& framebuffer) : m_Window(nullptr), m_Width(width), m_Height(height), m_Title(title), fbo(framebuffer)
{
    Logger::Log("Initialized Engine");
}

Core::~Core()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Core::Initialize()
{
    if (!InitializeGLFW()) return false;
    if (!InitializeOpenGL()) return false;
    return true;
}

void Core::BeginFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.GetFBO());
    glViewport(0, 0, fbo.GetWidth(), fbo.GetHeight());
    glClearColor(0.05f, 0.05f, 0.05f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Core::EndFrame()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Core::ShouldClose() const 
{
    return glfwWindowShouldClose(m_Window);
}

GLFWwindow* Core::GetWindow() const 
{
    return m_Window;
}

bool Core::InitializeGLFW()
{
    if (!glfwInit())
    {
        Logger::Log("FAILED TO INITIALIZE GLFW");
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    Logger::Log("Initialized GLFW corectly");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_Window);
    return true;
}

bool Core::InitializeOpenGL() 
{
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::Log("Failed to initialize GLEW");
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    Logger::Log("Initialized OpenGL corectly");
    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_TEST);
    return true;
}