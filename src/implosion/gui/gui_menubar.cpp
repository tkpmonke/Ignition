#include "gui/gui.hpp"

namespace Implosion {
   void GUI::MenuBar(std::vector<Ignition::Object>* objects)
   {
      ImGui::BeginMainMenuBar();
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
         if (ImGui::MenuItem("Add Object"))
         {
            objects->push_back(AddObjectMenu()); 
         }
      }
   }
}  
