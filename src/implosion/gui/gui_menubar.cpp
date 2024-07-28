#include "gui/gui.hpp"

namespace Implosion {
   void GUI::MenuBar(std::vector<Ignition::Object>* objects)
   {
      if (ImGui::BeginMainMenuBar()) {
         if (ImGui::BeginMenu("Ignition"))
         {
            if (ImGui::MenuItem("Exit"))
            {
               exit(0);
            }
            ImGui::EndMenu();
         }
         if (ImGui::BeginMenu("Scene"))
         {
            ImGui::SeparatorText("Add Object");
            AddObjectMenu(objects); 

            ImGui::EndMenu();
         }
         ImGui::EndMainMenuBar();
      }
   }
}  
