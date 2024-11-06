#include "ViewportPanel.h"
#include "EclipseEngine/FrameBuffer.h"
#include <imgui.h>

ViewportPanel::ViewportPanel(const std::string& name, Framebuffer& framebuffer, bool visible)
    : Panel(name), m_Framebuffer(framebuffer)
{
    SetVisible(visible);
    m_FramebufferWidth = m_Framebuffer.GetWidth();
    m_FramebufferHeight = m_Framebuffer.GetHeight();
}

void ViewportPanel::Render()
{
    if (IsVisible()) {
        ImGui::Begin("Viewport");

        // Calculate the available content area in the ImGui panel
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        int width = static_cast<int>(availableSize.x);
        int height = static_cast<int>(availableSize.y);

        // Resize the framebuffer if the ImGui panel size changes
        if (width != m_FramebufferWidth || height != m_FramebufferHeight) {
            m_FramebufferWidth = width;
            m_FramebufferHeight = height;
            m_Framebuffer.Resize(width, height);
        }

        // Display the framebuffer's texture in ImGui
        ImGui::Image((void*)(intptr_t)m_Framebuffer.GetTextureID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
}

void ViewportPanel::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Framebuffer.Resize(width, height);
}