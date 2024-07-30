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
         std::cout << "1\n";                                                                       \
         Shader s = Shader(unlit_vertex, unlit_fragment, ShaderType::Unlit);                       \
         std::cout << "2\n";                                                                       \
         Ignition::Rendering::Texture tex;                                                         \
         std::cout << "3\n";                                                                       \
         tex.SetFlags(TextureFlags::Repeat | TextureFlags::Linear);                                \
         std::cout << "4\n";                                                                       \
         FS::_Read texdata = FS::Read("./crate.png", FS::_Type::Texture, &tex);                    \
         std::cout << "5\n";                                                                       \
         s.albedo = tex.location;                                                                  \
         std::cout << "6\n";                                                                       \
         o.name = "Object " + std::to_string(objects->size());                                     \
         std::cout << "7\n";                                                                       \
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
         

         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         o.AddComponent(ptr);
         objects->push_back(o);
      }
   }
}
