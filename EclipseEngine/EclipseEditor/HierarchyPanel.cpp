#include "EclipseEngine/GameObject.h"
#include "App.h"
#include "HierarchyPanel.h"
#include "PanelHandler.h"
#include "imgui.h"

HierarchyPanel::HierarchyPanel(const std::string& name, bool isVisible) : Panel(name)
{
	SetVisible(isVisible);
	if (core->scene != nullptr)
	{
		m_RootObjects = core->scene->GetObjects();
	}
}

void HierarchyPanel::Render()
{
	if (!IsVisible()) return;

	ImGui::Begin(GetName().c_str(), &m_Visible);

	if (core->scene != nullptr)
	{
		m_RootObjects = core->scene->GetObjects();
	}

    if(ImGui::Button("Add Object")) 
    {
		core->scene->AddCube();
	}

    //for (const auto& rootObject : m_RootObjects) {
    //    RenderGameObjectTree(rootObject);
    //    if (ImGui::TreeNode(rootObject->GetName().c_str())) {
    //        // Optionally display details or components of the GameObject
    //        ImGui::TreePop();
    //    }
    //}

	for (const auto& rootObject : m_RootObjects) {
		RenderGameObjectTree(rootObject);
	}
	
	ImGui::End();
}

void HierarchyPanel::RemoveObject(std::shared_ptr<GameObject> rootObject)
{
    // TODO: delete method
}

void HierarchyPanel::AddRootObject(std::shared_ptr<GameObject> rootObject) {
	m_RootObjects.emplace_back(rootObject);
}

void HierarchyPanel::RenderGameObjectTree(const std::shared_ptr<GameObject>& gameObject) {
  
    // Check if this GameObject is the selected one
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (gameObject == m_SelectedObject) {
        flags |= ImGuiTreeNodeFlags_Selected; // Highlight if selected
    }

	// Create a tree node for the current game object
    if (ImGui::TreeNode(gameObject->GetName().c_str())) {  // Use a GetName() method to retrieve the object name

        // If selectable, add logic here to mark it as selected when clicked
        if (ImGui::IsItemClicked()) {
            // Handle selection (you may need to add selection logic, e.g., storing a reference to selected object)
			m_SelectedObject = gameObject;
			Logger::Log("Selected object: " + gameObject->GetName());

			if (m_InspectorPanel != nullptr)
			{
				m_InspectorPanel->SetSelectedObject(m_SelectedObject);
			}
			if (m_ViewportPanel != nullptr)
			{
				m_ViewportPanel->SetSelectedObject(m_SelectedObject);
			}
        }

		// Recursively render the children of the current game object
		for (const auto& child : gameObject->GetChildren()) {
			RenderGameObjectTree(child);
		}

        ImGui::TreePop();
    }
}