#include "gui/gui.hpp"
#include "scene.hpp"

#include <algorithm>

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

      for (auto& o : *obj->GetChildren())
      {
         ProcessNode(o, gui);
      }

      ImGui::TreePop();
   }

   if (ImGui::BeginDragDropSource()) {
      ImGui::SetDragDropPayload("SCENE_OBJECT", obj, sizeof(Ignition::Object*));
      ImGui::Text("%s", obj->name.data());
      ImGui::EndDragDropSource();
   }

   if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_OBJECT")) {
         Ignition::Object* object = static_cast<Ignition::Object*>(payload->Data);

         if (object != obj && object->parent != obj) {
            if (object->parent) {
               std::vector<Ignition::Object*>* siblings = object->parent->GetChildren();
               auto pos = std::find(siblings->begin(), siblings->end(), object);
               if (pos != siblings->end())
                  siblings->erase(pos);
            }

            obj->AddChild(object);
         }
      }
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
