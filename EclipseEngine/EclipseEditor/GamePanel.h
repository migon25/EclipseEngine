#ifndef GAME_PANEL_H
#define GAME_PANEL_H

#include "EclipseEngine/Framebuffer.h"
#include "Panel.h"

class GamePanel : public Panel
{
public:
    GamePanel(const std::string& name, Framebuffer* framebuffer, bool visible = true);
    ~GamePanel() {}

    void Render() override;

    void Resize(int width, int height);

private:
    Framebuffer* m_Framebuffer;
    int m_FramebufferWidth, m_FramebufferHeight;
    int m_Width;
    int m_Height;
};

#endif // GAME_PANEL_H
