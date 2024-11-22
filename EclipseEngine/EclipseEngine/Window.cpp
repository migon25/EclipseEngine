#include "Window.h"
#include "Logger.h"

Window::Window(int width, int height, const std::string& title) : m_Width(width), m_Height(height), m_Title(title)
{

}

Window::~Window()
{
	CleanUp();
}

bool Window::Initialize()
{
	if (!InitializeGLFW()) return false;
	if (!InitializeOpenGL()) return false;
	return true;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::CleanUp()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::InitializeGLFW()
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

    m_Monitor = glfwGetPrimaryMonitor();
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

bool Window::InitializeOpenGL()
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
