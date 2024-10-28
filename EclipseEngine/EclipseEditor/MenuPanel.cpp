#include "MenuPanel.h"
#include <imgui.h>

MenuPanel::MenuPanel(const std::string& name, PanelHandler& panelHandler)
    : Panel(name), m_PanelHandler(panelHandler) {}

void MenuPanel::Render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Panels")) {
            bool isBasicPanelVisible = m_PanelHandler.GetPanelVisibility("Basic Panel"); // Check visibility
            ImGui::MenuItem("Basic Panel", nullptr, &isBasicPanelVisible);
            if (isBasicPanelVisible != m_PanelHandler.GetPanelVisibility("Basic Panel")) {
                m_PanelHandler.TogglePanel("Basic Panel"); // Toggle the Basic Panel's visibility
            }

            // Toggle for the FPS panel
            bool isFPSPanelVisible = m_PanelHandler.GetPanelVisibility("FPS Panel");
            ImGui::MenuItem("FPS Panel", nullptr, &isFPSPanelVisible);
            if (ImGui::IsItemClicked()) {
                m_PanelHandler.TogglePanel("FPS Panel");
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
