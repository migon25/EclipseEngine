
#include <filesystem>
#include <imgui.h>

#include "EclipseEngine/Logger.h"
#include "EclipseEngine/GameObject.h" 
#include "EclipseEngine/Mesh.h"   

#include "App.h"
#include "tinyfiledialogs.h"
#include "AssetsPanel.h"

AssetsPanel::AssetsPanel(const std::string& name, bool isVisible) : Panel(name), m_CurrentDirectory("Assets")
{
    SetVisible(isVisible);
    m_FolderIcon = std::make_unique<Texture>("Assets/Editor/folder.png", "icon", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    m_FileIcons[".png"] = std::make_unique<Texture>("Assets/Editor/png.png", "icon", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    m_FileIcons[".jpg"] = std::make_unique<Texture>("Assets/Editor/jpg.png", "icon", 0, GL_RGBA, GL_UNSIGNED_BYTE);
    m_FileIcons[".fbx"] = std::make_unique<Texture>("Assets/Editor/fbx.png", "icon", 0, GL_RGBA, GL_UNSIGNED_BYTE);

    m_DefaultFileIcon = std::make_unique<Texture>("Assets/Editor/idk.png", "icon", 0, GL_RGBA, GL_UNSIGNED_BYTE);
}

void AssetsPanel::Render()
{
    if (!m_Visible) return;

    ImGui::Begin(m_Name.c_str(), &m_Visible);

    RenderBreadcrumbNavigation();

    const float iconSize = 80.0f;
    const int columns = 4; // Number of columns
    const float padding = 10.0f; // Padding between items
    
    if (ImGui::BeginTable("FileGrid", columns, ImGuiTableFlags_NoBordersInBody))
    {
        // Iterate over files in the Assets directory
        for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            const auto& path = entry.path();
            std::string fileName = path.filename().string();
    
            ImGui::TableNextColumn();
            ImGui::PushID(fileName.c_str());
    
            if (entry.is_directory())
            {
                // Render folder name as a selectable item
                if (ImGui::ImageButton((void*)(intptr_t)m_FolderIcon->textureID, ImVec2(iconSize, iconSize)))
                {
                    m_CurrentDirectory = path;
                }
                if (ImGui::BeginPopupContextItem("FolderContextMenu"))
                {
                    if (ImGui::MenuItem("Rename"))
                    {
                        m_IsRenaming = true;
                        m_RenamingPath = path;
                        m_RenameBuffer = fileName; // Pre-fill buffer with the current name
                    }
    
                    if (ImGui::MenuItem("Delete"))
                    {
                        m_ShowDeletePopup = true;
                        m_DeletePath = path;
                    }
    
                    ImGui::EndPopup();
                }
                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
                    {
                        // Get the source path from the payload
                        std::string sourcePath((const char*)payload->Data);
    
                        // Construct the target path inside this folder
                        std::string targetPath = (path / std::filesystem::path(sourcePath).filename()).string();
    
                        try
                        {
                            // Move the file
                            std::filesystem::rename(sourcePath, targetPath);
                            Logger::Log("Moved file to: " + targetPath);
                        }
                        catch (const std::exception& e)
                        {
                            Logger::Log("Error moving file: " + std::string(e.what()));
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    const std::string pathStr = path.string();
                    ImGui::SetDragDropPayload("ASSET_FILE", pathStr.c_str(), pathStr.size() + 1);
                    ImGui::Text("Dragging: %s", fileName.c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::TextWrapped("%s", fileName.c_str());
            }
            else
            {
                // Determine the file icon
                const std::string extension = path.extension().string();
                GLuint textureID;

                if (m_FileIcons.find(extension) != m_FileIcons.end())
                {
                    // Use specific icon for the file type
                    textureID = m_FileIcons[extension]->textureID;
                }
                else
                {
                    // Fallback to the default file icon
                    textureID = m_DefaultFileIcon->textureID;
                }

                if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), ImVec2(iconSize, iconSize)))
                {
                    // Handle image click (if necessary)
                    Logger::Log("Image clicked: " + fileName);
                }
                // Context menu for files
                if (ImGui::BeginPopupContextItem("FileContextMenu"))
                {
                    if (ImGui::MenuItem("Rename"))
                    {
                        m_IsRenaming = true;
                        m_RenamingPath = path;
                        m_RenameBuffer = fileName; // Pre-fill buffer with the current name
                    }
    
                    if (ImGui::MenuItem("Delete"))
                    {
                        m_ShowDeletePopup = true;
                        m_DeletePath = path;
                    }
    
                    ImGui::EndPopup();
                }
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    const std::string pathStr = path.string();
                    ImGui::SetDragDropPayload("FBX_FILE", pathStr.c_str(), pathStr.size() + 1);
                    ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), ImVec2(iconSize, iconSize));
                    ImGui::Text("%s", fileName.c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::Text("%s", fileName.c_str());
            }
            ImGui::PopID();
            if (m_IsRenaming && path == m_RenamingPath)
            {
                char renameBuffer[256];
                strncpy_s(renameBuffer, sizeof(renameBuffer), m_RenameBuffer.c_str(), _TRUNCATE);
    
                if (ImGui::InputText("##Rename", renameBuffer, sizeof(renameBuffer)))
                {
                    m_RenameBuffer = renameBuffer; // Update the buffer
                }
    
                ImGui::SameLine();
                if (ImGui::Button("Apply"))
                {
                    try
                    {
                        auto newPath = m_RenamingPath.parent_path() / m_RenameBuffer;
                        std::filesystem::rename(m_RenamingPath, newPath);
                        Logger::Log("Renamed file/folder: " + m_RenamingPath.string() + " to " + newPath.string());
                    }
                    catch (const std::exception& e)
                    {
                        Logger::Log("Error renaming: " + std::string(e.what()));
                    }
    
                    m_IsRenaming = false;
                }
    
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    m_IsRenaming = false;
                }
            }
        }
        ImGui::EndTable();
    }
    // Button to open the file explorer
    if (ImGui::Button("Open File Explorer"))
    {
        OpenFileExplorer();
    }

    // Handle drag and drop from file explorer
    HandleDragAndDrop();
    RenderDeletePopUp();
    ImGui::End();
}

void AssetsPanel::RenderBreadcrumbNavigation()
{
    std::vector<std::string> breadcrumbs;
    auto tempPath = m_CurrentDirectory;

    // Create breadcrumbs by splitting the path
    while (tempPath != tempPath.root_path() && tempPath != "Assets")
    {
        breadcrumbs.push_back(tempPath.filename().string());
        tempPath = tempPath.parent_path();
    }
    breadcrumbs.push_back("Assets"); // Add the base directory

    // Reverse breadcrumbs to display them in proper order
    std::reverse(breadcrumbs.begin(), breadcrumbs.end());

    // Render breadcrumbs
    for (size_t i = 0; i < breadcrumbs.size(); ++i)
    {
        if (ImGui::Button(breadcrumbs[i].c_str()))
        {
            // Navigate to the selected breadcrumb
            m_CurrentDirectory = std::filesystem::path("Assets");
            for (size_t j = 1; j <= i; ++j) // Rebuild path up to this breadcrumb
            {
                m_CurrentDirectory /= breadcrumbs[j];
            }
        }

        if (i < breadcrumbs.size() - 1)
        {
            ImGui::SameLine();
            ImGui::Text(">");
            ImGui::SameLine();
        }
    }
}

void AssetsPanel::OpenFileExplorer()
{
    const char* filters[] = { "*.fbx" };
    const char* selectedFile = tinyfd_openFileDialog(
        "Select an FBX File",               // Dialog title
        "Assets/",                          // Default path
        1,                                  // Number of filters
        filters,                            // File type filters
        "FBX Files",                        // Description of filters
        0                                   // Allow multiple selection (0 = No)
    );

    if (selectedFile)
    {
        std::filesystem::path path(selectedFile);
        if (!std::filesystem::exists(path))
        {
            Logger::Log("File not found: " + std::string(selectedFile));
            return;
        }

        try
        {
            // Check file extension and load as GameObject
            if (path.extension() == ".fbx")
            {
                auto gameObject = std::make_unique<GameObject>();
                gameObject->AddComponent<Mesh>(selectedFile); // Load the FBX file
                auto& meshTexture = gameObject->GetComponent<Mesh>()->GetTextures();
				gameObject->AddComponent<Material>(*core->scene->defaultShader, meshTexture); // Add a default material
                Logger::Log("Loaded FBX: " + std::string(selectedFile));
				core->scene->AddGameObject(std::move(gameObject));
                // Add the gameObject to your scene or collection of game objects
            }
            else
            {
                Logger::Log("Invalid file type: " + path.extension().string());
            }
        }
        catch (const std::exception& e)
        {
            Logger::Log("Error loading mesh: " + std::string(e.what()));
        }
    }
}

void AssetsPanel::RenderDeletePopUp()
{
    if (m_ShowDeletePopup)
    {
        ImGui::OpenPopup("Delete Confirmation");
    }

    if (ImGui::BeginPopupModal("Delete Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        std::string fileName = m_DeletePath.filename().string();
        ImGui::Text("Are you sure you want to delete '%s'?", fileName.c_str());
        if (ImGui::Button("Yes"))
        {
            try
            {
                if (std::filesystem::is_directory(m_DeletePath))
                {
                    std::filesystem::remove_all(m_DeletePath); // Delete folder and contents
                }
                else
                {
                    std::filesystem::remove(m_DeletePath); // Delete file
                }
                Logger::Log("Deleted: " + fileName);
            }
            catch (const std::exception& e)
            {
                Logger::Log("Error deleting: " + std::string(e.what()));
            }

            m_ShowDeletePopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("No"))
        {
            m_ShowDeletePopup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
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
                        Logger::Log("Dragging FBX file: " + path.string());
                        // Pass the payload to ViewportPanel when dropped there
                        ImGui::SetDragDropPayload("FBX_MODEL", filePath, strlen(filePath) + 1);
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