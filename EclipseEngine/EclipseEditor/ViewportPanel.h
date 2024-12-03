#ifndef VIEWPORTPANEL_H
#define VIEWPORTPANEL_H

#include <memory>
#include <string>
#include <filesystem>
#include "EclipseEngine/Framebuffer.h"
#include "EclipseEngine/Camera.h"
#include "EclipseEngine/GameObject.h"
#include "EclipseEngine/Texture.h"

#include "ImGuizmo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Panel.h"
#include "PanelHandler.h"
#include "InspectorPanel.h"

class ViewportPanel : public Panel
{
public:
    ViewportPanel(const std::string& name, Framebuffer* framebuffer, Camera* camera, bool visible = true);
    ~ViewportPanel() {}

    void Render() override;
    void SetSelectedObject(const std::shared_ptr<GameObject>& selectedObject) { m_SelectedObject = selectedObject; }

    void Resize(int width, int height);

private:
    Framebuffer* m_Framebuffer;
    Camera* m_camera;
	std::shared_ptr<GameObject> m_SelectedObject = nullptr;
};

#endif // VIEWPORTPANEL_H