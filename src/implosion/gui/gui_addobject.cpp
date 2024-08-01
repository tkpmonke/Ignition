#include "gui/gui.hpp"
#include "components/rendering/meshrenderer.hpp"
#include "types/shader.hpp"
#include "utils/unlit_shader.hpp"
#include "types/texture.hpp"

#include "shapes/square.hpp"
#include "shapes/cube.hpp"

#include "textures/grid.hpp"

#include <memory>

using namespace Ignition::Rendering;

#define CREATE_OBJECT(objects)                                                                     \
         Ignition::Object o = Ignition::Object();                                                  \
         Shader s = Shader(unlit_vertex, unlit_fragment, ShaderType::Unlit);                       \
         s.albedo = Texture();                                                                           \
         s.albedo.SetFlags(TextureFlags::Repeat | TextureFlags::Nearest);                                \
         s.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "ignition_grid_texture");                                                               \
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
         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         o.AddModule(ptr);
         objects->push_back(o);
      }
      if (ImGui::MenuItem("Cube"))
      {
         CREATE_OBJECT(objects); 
         MeshRenderer m = MeshRenderer(this->camera);
         m.LoadModel(cube_model);
         m.LoadShader(s);
         glUseProgram(m.shader.program);
         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         o.AddModule(ptr);
         objects->push_back(o);
      }
   }
}
