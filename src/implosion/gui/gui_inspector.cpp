#include "gui/gui.hpp"
#include "imgui_stdlib.h"
#include <glm/gtc/type_ptr.hpp>

#include "modules/rendering/meshrenderer.hpp"
#include "types/shader.hpp"
#include "utils/default_shaders.hpp"
#include "types/texture.hpp"
#include "shapes/square.hpp"
#include "shapes/cube.hpp"
#include "textures/grid.hpp"
#include "utils/model_loader.hpp"
#include "utils/files.hpp"
#include "modules/script.hpp"

#include <cstring>

#define GUI_BEGIN_DROP_TARGET if (ImGui::BeginDragDropTarget()) { \
                                 if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("__FILE_EXPLORER_")) { 

#define GUI_END_DROP_TARGET                                                  \
                           }                                                                                      \
                           ImGui::EndDragDropTarget();                                                            \
                        }

namespace Implosion {
   void GUI::Inspector()
   {
      Ignition::Object* obj = selectedObject;
      if (ImGui::Begin("Inspector")) {
         if (obj == nullptr)
         {
            ImGui::Text("Select Something to Use This");
         } else {
            
            ImGui::Checkbox("##Enabled", &obj->enabled);
            
            ImGui::SameLine();
            
            ImGui::InputText("Name", &obj->name);
            

            ImGui::InputText("Tag", &obj->tag);
            
            ImGui::Separator();

            ImGui::BeginChild("Transform", ImVec2(0, 75));
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
            }
            ImGui::EndChild();
            ImGui::Separator();

            for (std::shared_ptr<Ignition::Module> mod : obj->GetModules()) {
               if (mod->mod_type() == "Mesh Renderer") {
                  if (ImGui::CollapsingHeader("Mesh Renderer")) {
                     auto m = std::dynamic_pointer_cast<Ignition::Rendering::MeshRenderer>(mod);
                     ImGui::BeginChild("##Mesh_Renderer", ImVec2(0, 100));

                     if (ImGui::BeginCombo("Mesh", m->model.name == "" ? "Mesh" : m->model.name.data())) {
                        if (ImGui::Button("None")) {
                           m->LoadModel(Ignition::Model());
                        }

                        if (ImGui::Button("Square")) {
                           m->LoadModel(square_model);
                        }
                        
                        if (ImGui::Button("Cube")) {
                           m->LoadModel(cube_model);
                        }

                        ImGui::EndCombo();
                     }
                     GUI_BEGIN_DROP_TARGET
                        m->LoadModel(Ignition::ModelLoader::LoadModel((const char*)payload->Data));
                        selectedObject = nullptr;
                     GUI_END_DROP_TARGET

                     ImGui::InputText("Texture", &m->shader.albedo.name);
                     GUI_BEGIN_DROP_TARGET
                        std::string s = (const char*)payload->Data;
                        m->shader.albedo.LoadData(s.substr(Ignition::IO::GetProjectHome().size(), s.size()));
                     GUI_END_DROP_TARGET

                     ImGui::DragFloat("Intensity", &m->shader.intensity);
                     ImGui::EndChild();
                  }
               }

               if (mod->mod_type() == "Script") {
                  if (ImGui::CollapsingHeader("Script")) {
                     auto m = std::dynamic_pointer_cast<Ignition::Script>(mod);
                     ImGui::BeginChild("##Script_Component", ImVec2(0, 100));
                     ImGui::InputText("Path", &m->path);

                     GUI_BEGIN_DROP_TARGET
                        m->path = (const char*)payload->Data;
                     GUI_END_DROP_TARGET
                     ImGui::EndChild();
                  }
               }

            }

            if (ImGui::BeginCombo("##Add_Component", "Add Component")) {
               if (ImGui::Button("Mesh Renderer")) {
                  Ignition::Rendering::MeshRenderer m;
                  Ignition::Rendering::Shader s = 
                     Ignition::Rendering::Shader(unlit_vertex, unlit_fragment, Ignition::Rendering::ShaderType::Unlit);
                  s.albedo = Ignition::Rendering::Texture();
                  s.albedo.SetFlags(Ignition::Rendering::TextureFlags::Repeat | Ignition::Rendering::TextureFlags::Nearest);
                  s.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "Ignition_Grid");

                  m.LoadShader(s);
                  m.LoadModel(Ignition::Model());
                  auto ptr = std::make_shared<Ignition::Rendering::MeshRenderer>(m);
                  obj->AddModule(ptr);
               }

               if (ImGui::Button("Script")) {
                  std::cout << "hey\n";
                  Ignition::Script s;
                  std::cout << "hey\n";
                  auto ptr = std::make_shared<Ignition::Script>(s);
                  std::cout << "hey\n";
                  obj->AddModule(ptr);
                  std::cout << "hey\n";
               }
               ImGui::EndCombo();
            }
            
         }
         
      }

      ImGui::End();

      if (obj != nullptr)
      {

         Ignition::Rendering::MeshRenderer* renderer = (Ignition::Rendering::MeshRenderer*)obj->GetModule("Mesh Renderer"); 

         if (renderer != nullptr)
         {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(5);
            Ignition::Vector4 col = renderer->shader.color;
            renderer->shader.color = (Ignition::Vector4){1,1,1,0.75f}*.5f+.5f;
            float intensity = renderer->shader.intensity;
            renderer->shader.intensity = 1;
            renderer->Update();
            renderer->shader.color = col;
            renderer->shader.intensity = intensity;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         }
      }
   }
}

