#include "gui/gui.hpp"
#include "utils/files.hpp"
#include "project.hpp"

#ifdef __linux__
#include <unistd.h>
#endif

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
         
         if (ImGui::MenuItem("Build")) {
            Ignition::project.Build();
         }
         
         if (ImGui::MenuItem("Build And Run")) {
            auto s = Ignition::project.Build();
            Ignition::project.Run();
         }

         if (ImGui::MenuItem("Run")) {
            Ignition::project.Run();
         }

         std::string s = Ignition::IO::GetDateTime();
         ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(s.data()).x - ImGui::GetStyle().ItemSpacing.x - 3);
         ImGui::Text("%s", s.data());
         ImGui::EndMainMenuBar();
      }

      if (openAssetLoader == 1) {
         openAssetLoader = 0;
         this->InitAssetLoader();
      }
   }
}  
