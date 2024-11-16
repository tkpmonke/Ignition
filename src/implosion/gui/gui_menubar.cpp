#include "gui/gui.hpp"
#include "packer.hpp"
#include "lua/appinfo.hpp"
#include "project.hpp"

#include <filesystem>

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

            Ignition::scene.WriteSceneToDisk();
            Ignition::Scripting::Lua::LoadAppInfo();

            pack(Ignition::IO::GetProjectHome().data(),(Ignition::IO::GetProjectHome() + "/bin").data(), 1); 
            std::filesystem::copy_file("/usr/bin/ignition-runtime", Ignition::IO::GetProjectHome() + "/bin/"
                  + Ignition::Scripting::Lua::appInfo.appName);
            
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
