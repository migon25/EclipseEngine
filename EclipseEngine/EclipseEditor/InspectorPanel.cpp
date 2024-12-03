#include "imgui.h"
#include "glm/glm.hpp"

#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(const std::string& name, bool isVisible) : Panel (name)
{
    SetVisible(isVisible);
}

void InspectorPanel::Render()
{
    if (IsVisible())
    {
        ImGui::Begin(GetName().c_str(), &m_Visible);


        if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {   
            if (m_SelectedObject)
            {
                // Display the name of the selected object
                ImGui::Text("Selected Object: %s", m_SelectedObject->GetName().c_str());

                ImGui::Separator();

                // Render Transform controls (you already have this implemented)
                if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
                {
                    ImGui::Text("Transform");
                    ImGui::Separator();
                    glm::vec3 position = m_SelectedObject.get()->transform.position;
                    ImGui::SliderFloat3("Position", &position.x, -100.0f, 100.0f);

                    //// Update position in the selected object's transform
                    m_SelectedObject.get()->transform.SetPosition(position);

                    ImGui::Text("Scale");
                    glm::vec3 scale = m_SelectedObject.get()->transform.scale;
                    ImGui::SliderFloat3("Scale", &scale.x, 0.0f, 10.0f);
                    m_SelectedObject.get()->transform.SetScale(scale);

                    ImGui::Text("Rotation");
                    glm::vec3 rotation = m_SelectedObject.get()->transform.rotation;
                    ImGui::SliderFloat3("Rotation", &rotation.x, -180.0f, 180.0f);
                    m_SelectedObject.get()->transform.SetRotation(rotation);

                    ImGui::Separator();
                    (ImGui::Text("Mesh"));
                    ImGui::Separator();
                    bool temp = false;
                    ImGui::Checkbox("Active", &temp);
                    ImGui::SameLine();
                    ImGui::Text("File: ");
                    ImGui::Separator();
                    ImGui::Text("Draw:");
                    ImGui::Checkbox("Vertex Normals", &temp);
                    ImGui::Checkbox("Face Normals", &temp);
                    ImGui::SameLine();
                    ImGui::Separator();
                    ImGui::Text("Indexes: ");
                    ImGui::Text("Normals: ");
                    ImGui::Text("Vertexs: ");
                    ImGui::Text("Faces: ");
                    ImGui::Text("Tex coords: ");
                    //ImGui::Separator();
                    //ImGui::SliderFloat("Normals length", nullptr, 0.000f, 1.000f);
                    ImGui::Separator();

                    ImGui::Separator();
                    (ImGui::Text("Material"));
                    ImGui::Separator();
                    bool temp1;
                    ImGui::Checkbox("Active", &temp1);

                    ImGui::EndTabBar();
                }

            }
            else
            {
                // Display a placeholder message when no object is selected
                ImGui::Text("No object selected.");
            }

            
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
    
}
