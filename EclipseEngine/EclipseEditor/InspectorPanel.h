#pragma once
#include "Panel.h"
#include <vector>
#include <../EclipseEngine/EclipseEngine/GameObject.h>

class InspectorPanel :
    public Panel 
{
public:
    InspectorPanel(GameObject GameObject, const std::string& name);
};

