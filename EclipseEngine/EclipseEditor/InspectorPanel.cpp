#include "InspectorPanel.h"
#include "imgui.h"
#include "glm/glm.hpp"

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
            if (ImGui::CollapsingHeader("Transform"))
            {
                (ImGui::Text("Position"));
                glm::vec3 position(0, 0, 0);
                ImGui::SliderFloat3("position", &position.x, 0.0f, 960.0f);
                (ImGui::Text("Scale"));
                glm::vec3 Scale(0, 0, 0);
                ImGui::SliderFloat3("Scale", &Scale.x, 0.0f, 960.0f);
                (ImGui::Text("Rotation"));
                glm::vec3 Rotation(0, 0, 0);
                ImGui::SliderFloat3("Rotation", &Rotation.x, 0.0f, 960.0f);
            }

            if (ImGui::CollapsingHeader("Mesh"))
            {
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
                ImGui::Separator();
                //ImGui::SliderFloat("Normals length", nullptr, 0.000f, 1.000f);
                ImGui::Separator();
            }

            if (ImGui::CollapsingHeader("Material"))
            {
                bool temp1;
                ImGui::Checkbox("Active", &temp1);
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
    
}
