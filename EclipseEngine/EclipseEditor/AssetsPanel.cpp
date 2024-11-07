#include "AssetsPanel.h"
#include "EclipseEngine/Logger.h"
#include "EclipseEngine/GameObject.h" 
#include "EclipseEngine/Mesh.h"       
#include <filesystem>
#include <imgui.h>

AssetsPanel::AssetsPanel(const std::string& name, bool isVisible) : Panel(name) 
{
    SetVisible(isVisible);
}

void AssetsPanel::Render()
{
    if (!m_Visible) return;

    ImGui::Begin(m_Name.c_str(), &m_Visible);

    // Iterate over files in the Assets directory
    for (const auto& entry : std::filesystem::directory_iterator("Assets"))
    {
        const auto& path = entry.path();
        std::string fileName = path.filename().string();

        ImGui::Text("%s", fileName.c_str());
    }

    // Handle drag and drop from file explorer
    HandleDragAndDrop();

    ImGui::End();
}

void AssetsPanel::HandleDragAndDrop() 
{
    if (ImGui::BeginDragDropTarget()) 
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE")) 
        {
            if (payload->DataSize > 0)
            {
                const char* filePath = static_cast<const char*>(payload->Data);

                // Validate file path
                std::filesystem::path path(filePath);
                if (!std::filesystem::exists(path))
                {
                    Logger::Log("File not found: " + std::string(filePath));
                    return; // Return to avoid further processing
                }

                try 
                {
                    // Check file extension and load as GameObject
                    if (path.extension() == ".fbx") 
                    {
                        auto gameObject = std::make_unique<GameObject>();
                        gameObject->AddComponent<Mesh>(filePath); // Load the FBX file
                        Logger::Log("Loaded FBX: " + std::string(filePath));
                        // Here you should add the gameObject to your scene or collection of game objects
                    }
                }
                catch (const std::exception& e)
                {
                    Logger::Log("Error loading mesh: " + std::string(e.what()));
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
}