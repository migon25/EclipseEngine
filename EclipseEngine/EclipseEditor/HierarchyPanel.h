#ifndef HIERARCHY_PANEL_H
#define HIERARCHY_PANEL_H

#include <vector>
#include <memory>
#include "Panel.h"

class GameObject;

class HierarchyPanel : public Panel 
{
public:
	HierarchyPanel(const std::string& name, bool isVisible = false);

	void Render() override;
	void RemoveObject(std::shared_ptr<GameObject> rootObject);
	void AddRootObject(std::shared_ptr<GameObject> rootObject);

private:
	void RenderGameObjectTree(const std::shared_ptr<GameObject>& gameObject);

private:
	std::string m_Name;
	std::vector<std::shared_ptr<GameObject>> m_RootObjects;
};

#endif // HIERARCHY_PANEL_H