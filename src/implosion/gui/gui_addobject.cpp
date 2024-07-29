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

namespace Implosion {
   void GUI::AddObjectMenu(std::vector<Ignition::Object>* objects)
   {

      Ignition::Object o = Ignition::Object();
      Shader s = Shader(unlit_vertex, unlit_fragment, ShaderType::Unlit);
   
      Ignition::Rendering::Texture tex;
      tex.SetFlags(TextureFlags::Repeat | TextureFlags::Linear);
      FS::_Read texdata = FS::Read("./crate.png", FS::_Type::Texture, &tex);

      s.albedo = tex.location;
      o.name = "Object " + std::to_string(objects->size());
      o.tag = "Default";
      if (ImGui::MenuItem("Empty"))
      {
         objects->push_back(o);
      }
      if (ImGui::MenuItem("Square"))
      {
         MeshRenderer m = MeshRenderer(this->camera);
         m.LoadModel(square_model);
         m.LoadShader(s);
         

         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         o.AddComponent(ptr);
         objects->push_back(o);
      }
   }
}
