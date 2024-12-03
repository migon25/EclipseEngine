#ifndef ASSETSPANEL_H
#define ASSETSPANEL_H

#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>

#include "EclipseEngine/Texture.h"
#include "Panel.h"

class GameObject;

class AssetsPanel : public Panel
{
public:
    AssetsPanel(const std::string& name, bool visible = true);

    void Render() override;

    void HandleDragAndDrop();

private:
    void RenderBreadcrumbNavigation();
    void OpenFileExplorer();
    void RenderDeletePopUp();
    std::filesystem::path m_CurrentDirectory;

    // Renaming logic
    bool m_IsRenaming = false;             
    std::string m_RenameBuffer;            // Holds the new file name
    std::filesystem::path m_RenamingPath;  // The file path being renamed

	bool m_ShowDeletePopup = false;
    std::filesystem::path m_DeletePath;

	std::unique_ptr<Texture> m_FolderIcon;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_FileIcons;
    std::unique_ptr<Texture> m_DefaultFileIcon;
};

#endif // ASSETSPANEL_H
