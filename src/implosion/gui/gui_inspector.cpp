#include "gui/gui.hpp"
#include "imgui_stdlib.h"
#include <glm/gtc/type_ptr.hpp>

#include "modules/rendering/meshrenderer.hpp"
#include "types/shader.hpp"
#include "utils/default_shaders.hpp"
#include "types/texture.hpp"
#include "shapes/square.hpp"
#include "shapes/cube.hpp"
#include "shapes/ico_sphere.hpp"
#include "textures/grid.hpp"
#include "utils/model_loader.hpp"
#include "utils/files.hpp"
#include "modules/script.hpp"
#include "modules/physics/rigidbody.hpp"
#include "modules/rendering/light.hpp"
#include "camera.hpp"

#include <cstring>

#define VAR(x) ((std::string)x + "##" + std::to_string(mod->id)).data()

#define GUI_BEGIN_DROP_TARGET if (ImGui::BeginDragDropTarget()) { \
                                 if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("__FILE_EXPLORER_")) { 

#define GUI_END_DROP_TARGET                                                  \
                           }                                                                                      \
                           ImGui::EndDragDropTarget();                                                            \
                        }


#define MODULE_HEADER_MENU(name) \
   if (ImGui::BeginPopupContextItem((name+(std::string)"_"+mod->mod_type()).data())) {\
      if (ImGui::MenuItem(("Remove " + mod->mod_type()).data())) {\
         obj->RemoveModule(mod->mod_type());\
         ImGui::EndPopup();\
         ImGui::End();\
         return;\
      }\
      ImGui::EndPopup();\
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
               if (ImGui::CollapsingHeader(VAR(mod->mod_type()))) {
                  ImGui::Checkbox(("Enabled##"+mod->mod_type()).data(), &mod->enabled);
                  ImGui::Separator();
                  if (mod->mod_type() == "Mesh Renderer") {
                     auto m = std::dynamic_pointer_cast<Ignition::Rendering::MeshRenderer>(mod);
                     ImGui::BeginChild(VAR("##Mesh_Renderer"), ImVec2(0, 150));

                     if (ImGui::BeginCombo(VAR("Mesh"), m->model.name == "" ? "Mesh" : m->model.name.data())) {
                        if (ImGui::Button("None")) {
                           m->LoadModel(Ignition::Model());
                        }

                        if (ImGui::Button("Square")) {
                           m->LoadModel(square_model);
                        }
                        
                        if (ImGui::Button("Cube")) {
                           m->LoadModel(cube_model);
                        }
                        
                        if (ImGui::Button("Ico Sphere")) {
                           m->LoadModel(ico_sphere);
                        }

                        ImGui::EndCombo();
                     }
                     GUI_BEGIN_DROP_TARGET
                        m->LoadModel(Ignition::ModelLoader::LoadModel((const char*)payload->Data));
                        selectedObject = nullptr;
                     GUI_END_DROP_TARGET

                     ImGui::Spacing();

                     ImGui::InputText(VAR("Texture"), &m->shader.albedo.name);
                     GUI_BEGIN_DROP_TARGET
                        std::string s = (const char*)payload->Data;
                        m->shader.albedo.LoadData(s.substr(Ignition::IO::GetProjectHome().size(), s.size()));
                     GUI_END_DROP_TARGET

                     if (ImGui::BeginCombo(VAR("Texture Flags"), "Select Flags")) {
                        bool repeat =           m->shader.albedo.flags & Ignition::Rendering::TextureFlags_Repeat;
                        bool clamp =            m->shader.albedo.flags & Ignition::Rendering::TextureFlags_Clamp;
                        bool mirroredRepeat =   m->shader.albedo.flags & Ignition::Rendering::TextureFlags_Mirrored_Repeat;
                        bool nearest =          m->shader.albedo.flags & Ignition::Rendering::TextureFlags_Nearest;
                        bool linear =           m->shader.albedo.flags & Ignition::Rendering::TextureFlags_Linear;

                        if (ImGui::Checkbox("Repeat", &repeat)) {
                           if (repeat) m->shader.albedo.flags |= Ignition::Rendering::TextureFlags_Repeat;
                           else m->shader.albedo.flags &= ~Ignition::Rendering::TextureFlags_Repeat;
                           m->shader.albedo.LoadFlags();
                        }
                        if (ImGui::Checkbox("Clamp", &clamp)) {
                            if (clamp) m->shader.albedo.flags |= Ignition::Rendering::TextureFlags_Clamp;
                            else m->shader.albedo.flags &= ~Ignition::Rendering::TextureFlags_Clamp;
                           m->shader.albedo.LoadFlags();
                        }
                        if (ImGui::Checkbox("Mirrored Repeat", &mirroredRepeat)) {
                            if (mirroredRepeat) m->shader.albedo.flags |= Ignition::Rendering::TextureFlags_Mirrored_Repeat;
                            else m->shader.albedo.flags &= ~Ignition::Rendering::TextureFlags_Mirrored_Repeat;
                           m->shader.albedo.LoadFlags();
                        }
                        ImGui::Separator();
                        if (ImGui::Checkbox("Nearest", &nearest)) {
                            if (nearest) m->shader.albedo.flags |= Ignition::Rendering::TextureFlags_Nearest;
                            else m->shader.albedo.flags &= ~Ignition::Rendering::TextureFlags_Nearest;
                           m->shader.albedo.LoadFlags();
                        }
                        if (ImGui::Checkbox("Linear", &linear)) {
                            if (linear) m->shader.albedo.flags |= Ignition::Rendering::TextureFlags_Linear;
                            else m->shader.albedo.flags &= ~Ignition::Rendering::TextureFlags_Linear;
                           m->shader.albedo.LoadFlags();
                        }

                        ImGui::EndCombo();
                     }

                     ImGui::Separator();
                     if (ImGui::BeginCombo(VAR("##Shader"), "Shader")) {
                        if (ImGui::Button("Unlit")) {
                           auto tex = m->shader.albedo;
                           m->LoadShader(Ignition::Rendering::Shader(unlit_vertex, unlit_fragment, Ignition::Rendering::ShaderType_Unlit));
                           m->shader.albedo = tex;
                        }

                        if (ImGui::Button("Lit")) {
                           auto tex = m->shader.albedo;
                           m->LoadShader(Ignition::Rendering::Shader(unlit_vertex, lit_fragment, Ignition::Rendering::ShaderType_Lit));
                           m->shader.albedo = tex;
                        }

                        ImGui::EndCombo();
                     }

                     for (int i = 0; i < 4; ++i)
                        ImGui::Spacing();


                     ImGui::ColorEdit4(VAR("Color"), glm::value_ptr(m->shader.color));
                     ImGui::DragFloat(VAR("Intensity"), &m->shader.intensity);

                     ImGui::EndChild();

                  } if (mod->mod_type() == "Script") {
                     auto m = std::dynamic_pointer_cast<Ignition::Script>(mod);
                     ImGui::BeginChild(VAR("##Script_Component"), ImVec2(0, 100));
                     ImGui::InputText(VAR("Path"), &m->path);

                     /*
                     ImGui::Separator();

                     for (auto& v : m->variables) {
                        if (v.type == LuaData::Float) {
                           ImGui::InputFloat(VAR(v.name), &v.f);
                        } else if (v.type == LuaData::String) {
                           ImGui::InputText(VAR(v.name), &v.s);
                        } else if (v.type == LuaData::Boolean) {
                           ImGui::Checkbox(VAR(v.name), &v.b);
                        }
                     }
                     */

                     GUI_BEGIN_DROP_TARGET
                        std::string s = (const char*)payload->Data;
                        m->path = s.substr(Ignition::IO::GetProjectHome().size(), s.size());
                     GUI_END_DROP_TARGET
                     ImGui::EndChild();

                  } if (mod->mod_type() == "Rigidbody") {
                     auto m = std::dynamic_pointer_cast<Ignition::Physics::Rigidbody>(mod);
                     ImGui::BeginChild(VAR("##Rigidbody_Component"), ImVec2(0, 150));
                     
                     ImGui::InputFloat(VAR("Mass"), &m->mass);
                     ImGui::InputFloat(VAR("Bounciness"), &m->bounciness);
                     ImGui::InputFloat(VAR("Drag"), &m->drag);
                     ImGui::Checkbox(VAR("Is Trigger"), &m->trigger);
                     ImGui::Checkbox(VAR("Is Static"), &m->_static);
                     
                     if (ImGui::BeginCombo(VAR("##Collider"), "Collider")) {
                        if (ImGui::Button("Empty")) {
                           m->collider.shapeType = Ignition::Physics::Collider::Empty;
                        }
                        if (ImGui::Button("Cube")) {
                           m->collider.shapeVariables.size = Vec3(m->object->transform.scale.x,
                                    m->object->transform.scale.y,
                                    m->object->transform.scale.z)/2;
                           m->collider.shapeType = Ignition::Physics::Collider::Cube;
                        }

                        if (ImGui::Button("Sphere")) {
                           m->collider.shapeVariables.radius = (m->object->transform.scale.x+
                                    m->object->transform.scale.y+
                                    m->object->transform.scale.z)/3;
                           m->collider.shapeType = Ignition::Physics::Collider::Sphere;
                        }

                        /*if (ImGui::Button("Mesh")) {
                           m->collider.shapeType = Ignition::Physics::Collider::Mesh;
                        }*/

                        ImGui::EndCombo();
                     }

                     if (m->collider.shapeType == Ignition::Physics::Collider::Cube) {
                        ImGui::InputFloat3(VAR("Size"), (float*)&m->collider.shapeVariables.size);
                     }

                     if (m->collider.shapeType == Ignition::Physics::Collider::Sphere) {
                        ImGui::InputFloat(VAR("Radius"), &m->collider.shapeVariables.radius);
                     }

                     ImGui::EndChild();

                  } if (mod->mod_type() == "Light") {
                     auto m = std::dynamic_pointer_cast<Ignition::Rendering::Light>(mod);
                     if (ImGui::BeginCombo(VAR("##LightType"), "Type of Light")) {
                        if (ImGui::Button("Spot")) {
                           m->type = Ignition::Rendering::Spot;
                        }
                        if (ImGui::Button("Point")) {
                           m->type = Ignition::Rendering::Point;
                        }

                        ImGui::EndCombo();
                     }

                     ImGui::InputFloat(VAR("Distance"), &m->distance);
                     ImGui::InputFloat3(VAR("Ambient"), (float*)&m->ambient);
                     ImGui::InputFloat3(VAR("Diffuse"), (float*)&m->diffuse);
                     ImGui::InputFloat3(VAR("Specular"), (float*)&m->specular);

                     if (m->type == Ignition::Rendering::Spot) {
                        ImGui::InputFloat(VAR("Cut Off"), &m->cutOff);
                        ImGui::InputFloat(VAR("Outer Cut Off"), &m->outerCutOff);
                     }
                  }
               }   
               MODULE_HEADER_MENU(VAR("MODULE_HEADER_MENU"))
            }

            if (ImGui::BeginCombo("##Add_Component", "Add Component")) {
                  if (ImGui::Button("Mesh Renderer")) {
                     Ignition::Rendering::MeshRenderer m;
                     Ignition::Rendering::Shader s = 
                        Ignition::Rendering::Shader(unlit_vertex, unlit_fragment, Ignition::Rendering::ShaderType_Unlit);
                     s.albedo = Ignition::Rendering::Texture();
                     s.albedo.SetFlags(Ignition::Rendering::TextureFlags_Repeat | Ignition::Rendering::TextureFlags_Nearest);
                     s.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "Ignition_Grid");

                     m.LoadShader(s);
                     m.LoadModel(Ignition::Model());
                     auto ptr = std::make_shared<Ignition::Rendering::MeshRenderer>(m);
                     obj->AddModule(ptr);
                  }

                  if (ImGui::Button("Script")) {
                     auto ptr = std::make_shared<Ignition::Script>();
                     obj->AddModule(ptr);
                  }

                  if (ImGui::Button("Rigidbody")) {
                     auto ptr = std::make_shared<Ignition::Physics::Rigidbody>();
                     obj->AddModule(ptr);
                  }

                  if (ImGui::Button("Light")) {
                     auto ptr = std::make_shared<Ignition::Rendering::Light>();
                     obj->AddModule(ptr);
                     Ignition::Rendering::lights.push_back(std::make_shared<Ignition::Rendering::Light>(*(Ignition::Rendering::Light*)obj->GetModule("Light").get()));
                  }
                  ImGui::EndCombo();
               }
         }

      }

      ImGui::End();

      if (obj != nullptr)
      {

         Ignition::Rendering::MeshRenderer* renderer = (Ignition::Rendering::MeshRenderer*)obj->GetModule("Mesh Renderer").get(); 

         if (renderer != nullptr)
         {
            glDisable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(20/glm::distance(Ignition::mainCamera->transform.position, obj->transform.position));
            Ignition::Vector4 col = renderer->shader.color;
            renderer->shader.color = (Ignition::Vector4){0.8706,0.4235,0.0353,.75f};
            float intensity = renderer->shader.intensity;
            renderer->shader.intensity = 1.25;
            renderer->transform = &obj->transform;
            renderer->Update();
            renderer->shader.color = col;
            renderer->shader.intensity = intensity;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
         }
      }
   }
}

