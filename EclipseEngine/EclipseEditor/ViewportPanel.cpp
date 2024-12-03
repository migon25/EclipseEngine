#include <imgui.h>
#include <ImGuizmo.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "EclipseEngine/FrameBuffer.h"

#include "App.h"
#include "ViewportPanel.h"


ViewportPanel::ViewportPanel(const std::string& name, Framebuffer* framebuffer, Camera* camera, bool visible)
	: Panel(name), m_Framebuffer(framebuffer), m_camera(camera)
{
    SetVisible(visible);
}

void ViewportPanel::Render()
{
    if (IsVisible())
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoMove*/;
        ImGui::Begin("Viewport", nullptr, windowFlags);

        bool isViewportHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup);
        if (isViewportHovered) m_camera->Inputs(core->window->GetWindow());

        // Get the available size of the viewport panel
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        int width = static_cast<int>(viewportPanelSize.x);
        int height = static_cast<int>(viewportPanelSize.y);

        // Resize the framebuffer if the panel size changes
        if (width != m_Framebuffer->GetWidth() || height != m_Framebuffer->GetHeight()) {
            m_Framebuffer->Resize(width, height);
            m_camera->width = width;
            m_camera->height = height;
        }

        // Display the framebuffer's texture
        ImVec2 viewportPosition = ImGui::GetCursorScreenPos(); // Position of the viewport in screen coordinates
        ImGui::Image((void*)(intptr_t)m_Framebuffer->GetTextureID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));


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

        }


            glm::mat4 viewMatrix = m_camera->cameraMatrix;
            glm::mat4 projectionMatrix = glm::mat4(1.0f);

        ImGui::End();
    }
}

void ViewportPanel::Resize(int width, int height)
{
    m_Framebuffer->Resize(width, height);
}