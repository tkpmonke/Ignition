#include "gui/gui.hpp"
#include "scene.hpp"

void ProcessNode(Ignition::Object* obj, Implosion::GUI* gui)
{
   if (ImGui::TreeNode((obj->name + "##" + std::to_string(obj->id)).data()))
   {
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
}

namespace Implosion {
   void GUI::SceneHierarchy() {
      if (ImGui::Begin("Scene Hierarchy"))
      {
         if (ImGui::TreeNodeEx(Ignition::scene.name.data(), ImGuiTreeNodeFlags_DefaultOpen))
         {
            for (auto& obj : *Ignition::scene.GetObjects())
            {
               ProcessNode(&obj, this);
            }
            
            ImGui::TreePop();
         }
      }
      ImGui::End();
   }
}
