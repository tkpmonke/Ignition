#include "gui/gui.hpp"
#include "components/rendering/meshrenderer.hpp"
#include "types/shader.hpp"
#include "utils/unlit_shader.hpp"
#include "types/texture.hpp"

#include "shapes/square.hpp"
#include "shapes/cube.hpp"

#include "textures/grid.hpp"

#include "scene.hpp"

#include <memory>

using namespace Ignition::Rendering;

#define CREATE_SHADER()                                                                     \
         Shader s = Shader(unlit_vertex, unlit_fragment, ShaderType::Unlit);                       \
         s.albedo = Texture();                                                                           \
         s.albedo.SetFlags(TextureFlags::Repeat | TextureFlags::Nearest);                                \
         s.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "ignition_grid_texture");              \

namespace Implosion {
   void GUI::AddObjectMenu()
   {


      if (ImGui::MenuItem("Empty"))
      {
         Ignition::scene.CreateObject();
      }
      if (ImGui::MenuItem("Square"))
      {
         CREATE_SHADER(); 
         MeshRenderer m = MeshRenderer(this->camera);
         m.LoadModel(square_model, "square");
         m.LoadShader(s);
         glUseProgram(m.shader.program);
         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject()).AddModule(ptr);
      }
      if (ImGui::MenuItem("Cube"))
      {
         CREATE_SHADER(); 
         MeshRenderer m = MeshRenderer(this->camera);
         m.LoadModel(cube_model, "cube");
         m.LoadShader(s);
         glUseProgram(m.shader.program);
         std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
         Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject()).AddModule(ptr);
      }
      if (ImGui::MenuItem("Performance Test"))
      {
         for (int i = 0; i < 1000; ++i)
         {
            CREATE_SHADER(); 
            MeshRenderer m = MeshRenderer(this->camera);
            m.LoadModel(cube_model, "cube");
            m.LoadShader(s);
            glUseProgram(m.shader.program);
            std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
            Ignition::Object* o = &Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject());
            int x = -10 + rand() % 20;
            int y = -10 + rand() % 20;
            int z = -10 + rand() % 20;
            o->AddModule(ptr);
            o->transform.position = Ignition::Vector3(x,y,z);

         }
      }
   }
}
