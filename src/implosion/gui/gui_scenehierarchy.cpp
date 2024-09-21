#include "gui/gui.hpp"
#include "scene.hpp"

void ProcessNode(Ignition::Object* obj, Implosion::GUI* gui)
{
   bool b = false;
   if (gui->selectedObject != nullptr && gui->selectedObject->id == obj->id)
      b = 1;

   if (b) {
      const float f = (float)255/9;
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.4f,.6f,1,255)); 
   }

   if (ImGui::TreeNode((obj->name + "##" + std::to_string(obj->id)).data()))
   {
      if (b) {
         ImGui::PopStyleColor();
         b = 0;
      }
      if (ImGui::IsItemClicked() || ImGui::IsItemFocused())
      {
         gui->selectedObject = obj;
      }

      for (auto& o : obj->GetChildren())
      {
         ProcessNode(o, gui);
      }
      ImGui::TreePop();
   }


   if (b) 
      ImGui::PopStyleColor();
}

namespace Implosion {
   void GUI::SceneHierarchy() {
      if (ImGui::Begin("Scene Hierarchy"))
      {
         if (ImGui::TreeNodeEx(Ignition::scene.name.data(), ImGuiTreeNodeFlags_DefaultOpen))
         {
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1,255)); 
            for (auto& obj : *Ignition::scene.GetObjects())
            {
               ProcessNode(&obj, this);
            }
            ImGui::PopStyleColor();
            
            ImGui::TreePop();
         }
      }
      ImGui::End();
   }
}
