#ifndef ASSETSPANEL_H
#define ASSETSPANEL_H

#include "Panel.h"
#include <string>

class GameObject;

class AssetsPanel : public Panel {
public:
    AssetsPanel(const std::string& name, bool visible = true);

    void Render() override;

    void HandleDragAndDrop();
};

#endif // ASSETSPANEL_H
