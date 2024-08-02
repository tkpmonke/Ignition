#include "gui/gui.hpp"
#include "input/camera_movement.hpp"

bool vsync = true;
bool anti = true;

namespace Implosion {
   void GUI::Preferences()
   {
      ImGui::Begin("Settings Menu");

      if (ImGui::BeginTabBar("Settings Tabs")) 
      {

         if (ImGui::BeginTabItem("Graphics"))
         {
            if (ImGui::Checkbox("Enable V-Sync", &vsync))
            {
               glfwSwapInterval(vsync ? 1 : 0);
            }
            if (ImGui::Checkbox("Anti-Aliasing", &anti))
            {
               if (anti)
                  glEnable(GL_MULTISAMPLE);
               else
                  glDisable(GL_MULTISAMPLE);
            }
            
            ImGui::EndTabItem();
         }

         if (ImGui::BeginTabItem("Editor"))
         {
            ImGui::SeparatorText("Grid");
            ImGui::InputFloat("Grid Spacing", &this->gridSpacing);
            if (ImGui::IsItemHovered())
            {
               ImGui::BeginTooltip();
               ImGui::Text("Space between each grid line");
               ImGui::EndTooltip();
            }

            ImGui::InputFloat("Grid Size", &this->gridSize);
            if (ImGui::IsItemHovered())
            {
               ImGui::BeginTooltip();
               ImGui::Text("How far out the grids go. If your lagging, this might be a good option to change");
               ImGui::EndTooltip();
            }

            ImGui::InputFloat("Grid Height", &this->gridHeight);
            if (ImGui::IsItemHovered())
            {
               ImGui::BeginTooltip();
               ImGui::Text("The Y level of the grid");
               ImGui::EndTooltip();
            }
            ImGui::Spacing();
            ImGui::ColorEdit3("Grid Color", &this->gridColor[0]);
            ImGui::Spacing();
            if (ImGui::Button("Remake Grid")) this->RemakeGrid();
#ifdef DEBUG
            ImGui::Text("Grid VAO = %d", this->gridVao);
            ImGui::Text("Grid Program = %d", this->gridProgram);
            ImGui::Text("Grid Vertex Count = %d", this->gridVertSize);
#endif   

            ImGui::SeparatorText("Camera");

            ImGui::SliderFloat("Sensitivity", &sensitivity, 0.025f, 10.f);
            ImGui::InputFloat("Speed", &movespeed);

            ImGui::EndTabItem();
         }

        ImGui::EndTabBar();
    }

    ImGui::End();
   }
}
