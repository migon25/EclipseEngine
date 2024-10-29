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

class PanelHandler {
public:
    PanelHandler(GLFWwindow* window);
    ~PanelHandler();

    void NewFrame();
    void Render();
    void EndFrame();
    void ShowDemoWindow(bool* p_open);

    // Panel management
    void InitializePanels();
    void AddPanel(std::unique_ptr<Panel> panel);
    void RenderPanels();
    bool TogglePanel(const std::string& name);
    bool GetPanelVisibility(const std::string& name);

    std::unique_ptr<Panel>& GetPanel(const std::string& name);

private:
    GLFWwindow* m_Window;
    std::vector<std::unique_ptr<Panel>> m_Panels;
};

#endif // PANELHANDLER_H