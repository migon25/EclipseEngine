#include "ConsolePanel.h"
#include <imgui.h>

ConsolePanel::ConsolePanel(const std::string& name, bool visible)
    : Panel(name) {
    SetVisible(visible);
}

void ConsolePanel::Render() {
    if (IsVisible()) {
        if (ImGui::Begin(GetName().c_str(), &m_Visible)) {
            for (const auto& log : m_Logs) {
                ImGui::Text("%s", log.c_str());
            }
            ImGui::End();
        }
    }
}

void ConsolePanel::Log(const std::string& message) {
    m_Logs.push_back(message);
    if (m_Logs.size() > 100) { // Keep the log size manageable
        m_Logs.erase(m_Logs.begin());
    }
}