#ifndef HIERARCHY_PANEL_H
#define HIERARCHY_PANEL_H

#include "Panel.h"
#include <vector>

class HierarchyPanel : public Panel 
{
public:
	HierarchyPanel(const std::string& name, bool isVisible = false);

	void Render() override;
	void AddObject(const std::string& objectName);
	void RemoveObject(const std::string& objectName);

private:
	std::string m_Name;
	std::vector<std::string> m_Objects;
};

#endif // HIERARCHY_PANEL_H