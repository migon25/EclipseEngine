#include "ViewportPanel.h"
#include "EclipseEngine/FrameBuffer.h"
#include <imgui.h>

ViewportPanel::ViewportPanel(const std::string& name, Framebuffer* framebuffer, Camera* camera, bool visible)
	: Panel(name), m_Framebuffer(framebuffer), m_camera(camera)
{
    SetVisible(visible);
    m_FramebufferWidth = m_Framebuffer->GetWidth();
    m_FramebufferHeight = m_Framebuffer->GetHeight();
}

void ViewportPanel::Render()
{
    if (IsVisible()) {
        ImGui::Begin("Viewport");

        const float aspectRatio = 16.0f /9.0f;

        // Calculate the available content area in the ImGui panel
        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        float availableWidth = availableSize.x;
        float availableHeight = availableSize.y;

        float viewportWidth = availableWidth / aspectRatio;
        float viewportHeight = availableWidth;

        if (viewportHeight > availableHeight) {
            // If the calculated height is too large, adjust to fit within the available height
            viewportHeight = availableHeight * aspectRatio;
            viewportWidth = availableHeight;
        }

        int width = static_cast<int>(availableSize.x);
        int height = static_cast<int>(availableSize.y);

        // Resize the framebuffer if the ImGui panel size changes
        if (width != m_FramebufferWidth || height != m_FramebufferHeight) {
            m_FramebufferWidth = width;
            m_FramebufferHeight = height;
            m_Framebuffer->Resize(width, height);

			m_camera->width = width;
			m_camera->height = height;
        }

        ImVec2 cursorPos = ImGui::GetCursorPos();
        float offsetX = (availableWidth - viewportWidth) / 2.0f;
        float offsetY = (availableHeight - viewportHeight) / 2.0f;

        // Display the framebuffer's texture in ImGui
        ImGui::Image((void*)(intptr_t)m_Framebuffer->GetTextureID(), ImVec2(m_FramebufferWidth, m_FramebufferHeight), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
}

void ViewportPanel::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_Framebuffer->Resize(width, height);
}