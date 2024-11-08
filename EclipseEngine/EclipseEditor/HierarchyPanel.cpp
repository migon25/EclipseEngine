#include "EclipseEngine/GameObject.h"
#include "HierarchyPanel.h"
#include "PanelHandler.h"
#include "imgui.h"

HierarchyPanel::HierarchyPanel(const std::string& name, bool isVisible) : Panel(name)
{
	SetVisible(isVisible);
}

void HierarchyPanel::Render()
{
	if (!IsVisible()) return;

	ImGui::Begin(GetName().c_str(), &m_Visible);

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
	m_RootObjects.push_back(rootObject);
}

void HierarchyPanel::RenderGameObjectTree(const std::shared_ptr<GameObject>& gameObject) {
    // Create a tree node for the current game object
    if (ImGui::TreeNode(gameObject->GetName().c_str())) {  // Use a GetName() method to retrieve the object name

        // If selectable, add logic here to mark it as selected when clicked
        if (ImGui::IsItemClicked()) {
            // Handle selection (you may need to add selection logic, e.g., storing a reference to selected object)
        }

        // Recurse for each child
        for (const auto& child : gameObject->GetChildren()) {
            RenderGameObjectTree(child);
        }

        ImGui::TreePop();
    }
}