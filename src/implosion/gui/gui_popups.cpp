#include "gui/gui.hpp"
#include "imgui_stdlib.h"
#include "utils/files.hpp"

const std::pair<const char*, int> typesOfAssets[] = {
   {"Texture", 0},
   {"Model", 1},
   {"Shader", 2}
};

namespace Implosion {
   int g = -1;
   std::string fileLocation;
   void RenderAssetLoader() {
      if (ImGui::BeginCombo("##Type Of Asset", g == -1 ? "Type Of Asset" : typesOfAssets[g].first))
      {
         for (int i = 0; i < 3; ++i)
         {
            bool selected = (i == g);
            if (ImGui::Selectable(typesOfAssets[i].first, selected))
            {
               g = i;
            }

         }
         ImGui::EndCombo();
      }

      if (ImGui::Button("Explore")) {
         fileLocation = FS::FileExplorer();
      }
      ImGui::SameLine();
      ImGui::InputText("Location", &fileLocation);

      if (ImGui::Button("Cancel"))
      {
         ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      ImGui::BeginDisabled(g == -1 || fileLocation == "");
      if (ImGui::Button("Create"))
      {
         

      }
      ImGui::EndDisabled();
   }

   void GUI::RenderPopups() {

      for (int i = 0; i < this->popups.size(); ++i)
      {
         if (ImGui::BeginPopup(popups[i].name))
         {
            typedef void (*FunctionType)();

            auto v = (FunctionType)popups[i].render;
            v();
            
            ImGui::EndPopup();
         } else {
            popups.erase(popups.begin()+i);
            i--;
         }
      }
   }

   
   void GUI::InitAssetLoader() {
      auto func = reinterpret_cast<void*>(RenderAssetLoader);
      popups.push_back({"Asset Loader", func});
      ImGui::OpenPopup("Asset Loader");
   }
}
