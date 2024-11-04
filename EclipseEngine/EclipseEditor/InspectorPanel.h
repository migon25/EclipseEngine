#ifndef INSPECTOR_PANEL_H
#define INSPECTOR_PANEL_H

#include "Panel.h"
#include <vector>

class InspectorPanel : public Panel {
public:
    InspectorPanel(const std::string& name, bool isVisible = false);;
    void Render() override;

};

#endif // INSPECTOR_PANEL_H