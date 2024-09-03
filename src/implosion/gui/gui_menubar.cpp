#include "gui/gui.hpp"

namespace Implosion {
   void GUI::MenuBar()
   {
      if (ImGui::BeginMainMenuBar()) {
         if (ImGui::BeginMenu("Implosion"))
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
            AddObjectMenu(); 

            ImGui::EndMenu();
         }
         ImGui::EndMainMenuBar();
      }


      if (openAssetLoader == 1) {
         openAssetLoader = 0;
         this->InitAssetLoader();
      }
   }
}  
