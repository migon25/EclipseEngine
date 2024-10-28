#ifndef MENU_PANEL_H
#define MENU_PANEL_H

#include "Panel.h"
#include "PanelHandler.h" // PanelHandler for toggling panels

class MenuPanel : public Panel {
public:
    MenuPanel(const std::string& name, PanelHandler& panelHandler);

    void Render() override;

    bool demo = true;

private:
    PanelHandler& m_PanelHandler; // Reference to PanelHandler
};

#endif // MENU_PANEL_H