#include "gui/gui.hpp"
#include "input/camera_movement.hpp"
#include "glm/gtc/type_ptr.hpp"

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

            ImGui::Checkbox("Enable Grid", &this->enableGrid);

            ImGui::Spacing();

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

            ImGui::DragFloat("Distance", &this->gridDistance);
            ImGui::SliderFloat("Distance Falloff", &this->gridFalloff, 1.1f, 10);
            ImGui::SameLine();
            ImGui::Checkbox("##Distance Falloff Enabled", &this->enableDistanceFalloff);
            ImGui::Spacing();
            ImGui::ColorEdit4("Grid Color", glm::value_ptr(this->gridColor));
            ImGui::Spacing();
            if (ImGui::Button("Remake Grid")) this->RemakeGrid();
#ifdef DEBUG
            ImGui::Text("Grid VAO = %d", this->gridVao);
            ImGui::Text("Grid Program = %d", this->gridProgram);
            ImGui::Text("Grid Vertex Count = %d", this->gridVertSize);
#endif   

            ImGui::SeparatorText("Camera");

            ImGui::SliderFloat("Sensitivity", &sensitivity, 0.025f, 1.f);
            ImGui::InputFloat("Speed", &movespeed);
            ImGui::InputFloat("Shift Speed Multiplier", &shiftMul); 

            ImGui::Spacing();

            ImGui::InputFloat("Near", &this->camera->clipping_planes.min);
            ImGui::InputFloat("Far", &this->camera->clipping_planes.max);
            ImGui::SliderFloat("FOV", &this->camera->fov, 15, 130);

            ImGui::EndTabItem();
         }

        ImGui::EndTabBar();
    }

    ImGui::End();
   }
}
