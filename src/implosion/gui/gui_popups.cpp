#include "gui/gui.hpp"

#include <iostream>

namespace Implosion {
   void RenderAssetLoader() {
      ImGui::Text("Fortnite");
      if (ImGui::Button("Cancel"))
      {
         ImGui::CloseCurrentPopup();
      }
   }

   void GUI::RenderPopups() {

      if (ImGui::Begin("hey"))
      {
         if(ImGui::Button("open")) {
            auto func = reinterpret_cast<void*>(RenderAssetLoader);
            popups.push_back({"Asset Loader", func});
            ImGui::OpenPopup("Asset Loader");
         }
      }

      for (int i = 0; i < this->popups.size(); ++i)
      {
         if (ImGui::BeginPopup(popups[i].name))
         {
            typedef void (*FunctionType)();

            auto v = (FunctionType)popups[i].render;
            v();
            
            ImGui::EndPopup();
         } else {
            popups.erase(popups.begin(), popups.begin()+i);
         }
      }
      ImGui::End();
   }

   
   void GUI::InitAssetLoader() {
      auto func = reinterpret_cast<void*>(RenderAssetLoader);
      popups.push_back({"Asset Loader", func});
      ImGui::OpenPopup("Asset Loader");
   }
}
