#include "gui/gui.hpp"
#include "modules/rendering/meshrenderer.hpp"
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
         s.albedo.LoadData((unsigned char*)grid_texture, 8, 8, 3, "ignition_grid_texture");              

namespace Implosion {
   void GUI::AddObjectMenu()
   {

      if (ImGui::BeginMenu("Create Object"))
      {
         if (ImGui::MenuItem("Empty"))
         {
            Ignition::scene.CreateObject();
         }
         if (ImGui::MenuItem("Square"))
         {
            CREATE_SHADER(); 
            MeshRenderer m = MeshRenderer(this->camera);
            m.LoadModel(square_model);
            m.LoadShader(s);
            std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
            Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject()).AddModule(ptr);
         }
         if (ImGui::MenuItem("Cube"))
         {
            CREATE_SHADER(); 
            MeshRenderer m = MeshRenderer(this->camera);
            m.LoadModel(cube_model);
            m.LoadShader(s);
            std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
            Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject()).AddModule(ptr);
         }
         ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Clear All Objects"))
      {
         Ignition::scene.GetObjects()->clear();
      }
#ifdef DEBUG
      ImGui::Separator();
      if (ImGui::BeginMenu("Debug")) { 
         if (ImGui::MenuItem("Performance Test"))
         {
            for (int i = 0; i < 1000; ++i)
            {
               CREATE_SHADER(); 
               MeshRenderer m = MeshRenderer(this->camera);
               m.LoadModel(cube_model);
               m.LoadShader(s);
               std::shared_ptr<MeshRenderer> ptr = std::make_shared<MeshRenderer>(m);
               Ignition::Object* o = &Ignition::scene.GetObjects()->at(Ignition::scene.CreateObject());
               int x = -50 + rand() % 100;
               int y = -50 + rand() % 100;
               int z = -50 + rand() % 100;

               int rx = -180 + rand() % 360;
               int ry = -180 + rand() % 360;
               int rz = -180 + rand() % 360;

               float sx = .5f + rand() % 2;
               float sy = .5f + rand() % 2;
               float sz = .5f + rand() % 2;

               o->AddModule(ptr);
               o->transform.position = Ignition::Vector3(x,y,z);
               o->transform.rotation = Ignition::Vector3(rx,ry,rz);
               o->transform.scale =    Ignition::Vector3(sx,sy,sz);

            }
         }
         ImGui::EndMenu();
      }
#endif
      
   }
}
