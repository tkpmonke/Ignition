#include "gui/gui.hpp"
#include "components/rendering/meshrenderer.hpp"
#include "shapes/square.hpp"
#include "types/shader.hpp"
#include "utils/unlit_shader.hpp"
#include "types/texture.hpp"
#include "utils/files.hpp"

#include <memory>
#include <iostream>
using namespace Ignition::Rendering;

#define CREATE_OBJECT(objects)                                                                     \
         Ignition::Object o = Ignition::Object();                                                  \
         Shader s = Shader(unlit_vertex, unlit_fragment, ShaderType::Unlit);                       \
         s.albedo = Texture();                                                                           \
         s.albedo.SetFlags(TextureFlags::Repeat | TextureFlags::Linear);                                \
         s.albedo.LoadData("./crate.png");                                                               \
         o.name = "Object " + std::to_string(objects->size());                                     \
         o.tag = "Default";                                                      

namespace Implosion {
   void GUI::AddObjectMenu(std::vector<Ignition::Object>* objects)
   {


      if (ImGui::MenuItem("Empty"))
      {
         Ignition::Object o;
         objects->push_back(o);
      }
      if (ImGui::MenuItem("Square"))
      {
         CREATE_OBJECT(objects); 
         MeshRenderer m = MeshRenderer(this->camera);
         m.LoadModel(square_model);
         m.LoadShader(s);
         
         
         glUseProgram(m.shader.program);
         std::cout << glGetError() << "\n";
         m.shader.SetInt(m.shader.albedo.id, "material.albedo");
         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         o.AddComponent(ptr);
         objects->push_back(o);
      }
   }
}
