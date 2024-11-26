#ifndef VIEWPORTPANEL_H
#define VIEWPORTPANEL_H

#include "EclipseEngine/Framebuffer.h"
#include "EclipseEngine/Camera.h"
#include "Panel.h"

class ViewportPanel : public Panel
{
public:
    ViewportPanel(const std::string& name, Framebuffer* framebuffer, Camera* camera, bool visible = true);
    ~ViewportPanel() {}

    void Render() override;

    void Resize(int width, int height);

private:
    Framebuffer* m_Framebuffer;
    Camera* m_camera;
    int m_FramebufferWidth, m_FramebufferHeight;
    int m_Width;
    int m_Height;
};

#endif // VIEWPORTPANEL_H
