#ifndef PANELHANDLER_H
#define PANELHANDLER_H

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory> // For std::unique_ptr
#include "Panel.h"
#include "EclipseEngine/FrameBuffer.h"

class PanelHandler
{
public:
    PanelHandler(GLFWwindow* window, Framebuffer& framebuffer);
    ~PanelHandler();

    void NewFrame();
    void Render();
    void EndFrame();
    void ShowDemoWindow(bool* p_open);
    void CustomStyle();

    // Panel management
    void InitializePanels();
    void AddPanel(std::shared_ptr<Panel> panel);
    void RenderPanels();
    bool TogglePanel(const std::string& name);
    bool GetPanelVisibility(const std::string& name);

    inline GLFWwindow* GetWindow() { return m_Window; }

    std::shared_ptr<Panel> GetPanel(const std::string& name);

private:
    GLFWwindow* m_Window;
    Framebuffer& m_Framebuffer;
    std::vector<std::shared_ptr<Panel>> m_Panels;
};

#endif // PANELHANDLER_H