#include "EclipseEngine/FrameBuffer.h"
#include <imgui.h>
#include "GamePanel.h"

GamePanel::GamePanel(const std::string& name, Framebuffer* framebuffer, bool visible) : Panel(name), m_Framebuffer(framebuffer)
{
    SetVisible(visible);
    m_FramebufferWidth = m_Framebuffer->GetWidth();
    m_FramebufferHeight = m_Framebuffer->GetHeight();
}

void GamePanel::Render()
{
	if (IsVisible()) {
		ImGui::Begin("Game");

		const float aspectRatio = 16.0f / 9.0f;

		// Calculate the available content area in the ImGui panel
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		float availableWidth = availableSize.x;
		float availableHeight = availableSize.y;

		float viewportWidth = availableWidth;
		float viewportHeight = availableWidth / aspectRatio;

		if (viewportHeight > availableHeight) {
			// If the calculated height is too large, adjust to fit within the available height
			viewportHeight = availableHeight;
			viewportWidth = availableHeight * aspectRatio;
		}

		int width = static_cast<int>(availableSize.x);
		int height = static_cast<int>(availableSize.y);

		// Resize the framebuffer if the ImGui panel size changes
		if (width != m_FramebufferWidth || height != m_FramebufferHeight) {
			m_FramebufferWidth = width;
			m_FramebufferHeight = height;
			m_Framebuffer->Resize(width, height);
		}

		ImVec2 cursorPos = ImGui::GetCursorPos();
		float offsetX = (availableWidth - viewportWidth) / 2.0f;
		float offsetY = (availableHeight - viewportHeight) / 2.0f;
		ImGui::SetCursorPos(ImVec2(cursorPos.x + offsetX, cursorPos.y + offsetY));

		// Display the framebuffer's texture in ImGui
		ImGui::Image((void*)(intptr_t)m_Framebuffer->GetTextureID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}

void GamePanel::Resize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	m_Framebuffer->Resize(width, height);
}
	