#include "HierarchyPanel.h"
#include "PanelHandler.h"
#include "imgui.h"

HierarchyPanel::HierarchyPanel(const std::string& name, bool isVisible) : Panel(name)
{
	SetVisible(isVisible);
	m_Objects.push_back("Object 1");
	m_Objects.push_back("Object 2");
	m_Objects.push_back("Object 3");
	m_Objects.push_back("Object 4");
}

void HierarchyPanel::Render()
{
	if (!IsVisible()) return;

	ImGui::Begin(GetName().c_str(), &m_Visible);
	for (const auto& object : m_Objects)
	{
		ImGui::Text(object.c_str());
		if (ImGui::Button("Remove"))
		{
			RemoveObject(object);
		};
	}
	
	ImGui::End();

}

void HierarchyPanel::AddObject(const std::string& objectName)
{
	m_Objects.push_back(objectName);
}

void HierarchyPanel::RemoveObject(const std::string& objectName)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), objectName), m_Objects.end());
}
