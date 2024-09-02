#include "gui/gui.hpp"
#include "modules/rendering/meshrenderer.hpp"

#include "imgui_stdlib.h"

#include <glm/gtc/type_ptr.hpp>

#include <cstring>
#include <iostream>

namespace Implosion {
   void GUI::Inspector()
   {
      Ignition::Object* obj = selectedObject;
      if (ImGui::Begin("Inspector")) {
         if (obj == nullptr)
         {
            ImGui::Text("Select Something to Use This");
         } else {
            
            Ignition::Rendering::MeshRenderer* renderer = (Ignition::Rendering::MeshRenderer*)obj->GetModule("Mesh Renderer"); 
            ImGui::Checkbox("##Enabled", &obj->enabled);
            
            ImGui::SameLine();
            
            ImGui::InputText("Name", &obj->name);
            ImGui::InputText("Tag", &obj->tag);
            
            ImGui::Separator();

            ImGui::BeginChild("Transform", ImVec2(0, 100));
            {
               Ignition::Vector3 pos = obj->transform.position;
               Ignition::Vector3 rot = obj->transform.rotation;
               Ignition::Vector3 sca = obj->transform.scale;
               if (ImGui::InputFloat3("Position", glm::value_ptr(pos)))
               {
                  obj->transform.position = pos;
               }
               if (ImGui::InputFloat3("Rotation", glm::value_ptr(rot)))
               {
                  obj->transform.rotation = rot;
               }
               if (ImGui::InputFloat3("Scale", glm::value_ptr(sca)))
               {
                  obj->transform.scale = sca;
               }
               
               if (ImGui::CollapsingHeader("Vectors"))
               {
                  ImGui::Text("Forward : %f, %f, %f", obj->transform.forward.x, obj->transform.forward.y, obj->transform.forward.z);
                  ImGui::Text("Right : %f, %f, %f", obj->transform.right.x, obj->transform.right.y, obj->transform.right.z);
                  ImGui::Text("Up : %f, %f, %f", obj->transform.up.x, obj->transform.up.y, obj->transform.up.z);
               }
            
            }
            ImGui::EndChild();
            ImGui::Separator();
            ImGui::BeginChild("Material");
            if (renderer != nullptr)
            {
               if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
               {
                  Ignition::Vector4 color = renderer->shader.color;
                  if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
                  {
                     renderer->shader.color = color;
                  }

                  ImGui::DragFloat("Intensity", &renderer->shader.intensity);
               }
            }
            else
            {
               std::cout << "Object Has No Renderer\n";
            }
            ImGui::EndChild();
            if (renderer != nullptr)
            {
               glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
               glLineWidth(5);
               Ignition::Vector4 col = renderer->shader.color;
               renderer->shader.color = {1,1,1,0.75f};
               renderer->Update();
               renderer->shader.color = col;
               glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
         }
         
      }
         
        

      ImGui::End();
   }
}

