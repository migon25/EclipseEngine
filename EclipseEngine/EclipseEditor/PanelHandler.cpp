#include <stdexcept>
#include "PanelHandler.h"
#include "MenuPanel.h"
#include "BasicPanel.h"
#include "FPSpanel.h"
#include "ConsolePanel.h"
#include "HierarchyPanel.h"
#include "SettingsPanel.h"

PanelHandler::PanelHandler(GLFWwindow* window) : m_Window(window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    InitializePanels();
}

PanelHandler::~PanelHandler() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PanelHandler::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void PanelHandler::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(m_Window);
    }
}

void PanelHandler::ShowDemoWindow(bool* p_open) {
    ImGui::ShowDemoWindow(p_open);
}

void PanelHandler::InitializePanels()
{
    AddPanel(std::make_unique<MenuPanel>("Menu Panel", *this));
    AddPanel(std::make_unique<BasicPanel>("Basic Panel", true));
    AddPanel(std::make_unique<FPSPanel>("FPS Panel", false)); 
    AddPanel(std::make_unique<ConsolePanel>("Console Panel", true));
	AddPanel(std::make_unique<HierarchyPanel>("Hierarchy Panel", true));
	AddPanel(std::make_unique<SettingsPanel>("Settings Panel", false));
}

void PanelHandler::AddPanel(std::unique_ptr<Panel> panel) {
    m_Panels.push_back(std::move(panel));
}

void PanelHandler::RenderPanels() {
    for (const auto& panel : m_Panels) {
        panel->Render();
    }
}

bool PanelHandler::TogglePanel(const std::string& name) {
    for (auto& panel : m_Panels) {
        if (panel->GetName() == name) {
            panel->SetVisible(!panel->IsVisible());
            return panel->IsVisible(); // Return new visibility state
        }
    }
    return false;
}

bool PanelHandler::GetPanelVisibility(const std::string& name) {
    for (const auto& panel : m_Panels) {
        if (panel->GetName() == name) {
            return panel->IsVisible();
        }
    }
    return false;
}

std::unique_ptr<Panel>& PanelHandler::GetPanel(const std::string& name) {
    for (auto& panel : m_Panels) {
        if (panel->GetName() == name) {
            return panel;
        }
    }
    throw std::runtime_error("Panel not found: " + name);
}