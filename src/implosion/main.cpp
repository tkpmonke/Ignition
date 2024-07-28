#include "window.hpp"
#include "camera.hpp"

#include "gui/gui.hpp"

#include "shapes/square.hpp"
#include "utils/unlit_shader.hpp"
#include "components/rendering/meshrenderer.hpp"

#include <iostream>


int main() 
{
   Ignition::Window window = Ignition::Window(1920, 1080, "Implosion");
   Ignition::Camera camera = Ignition::Camera(&window);
   camera.fov = 75;
   camera.clipping_planes.min = 0.1f;
   camera.clipping_planes.max = 100.f;
   camera.MakeMainCamera();

   std::vector<Ignition::Object> objects;

   Implosion::GUI gui = Implosion::GUI(window, &camera);
   /*
   Ignition::Object obj;
   Ignition::Rendering::MeshRenderer m;
   m.LoadModel(square_model);
   m.LoadShader(Ignition::Rendering::Shader(unlit_vertex, unlit_fragment));
   
   obj.AddComponent(std::make_shared<Ignition::Rendering::MeshRenderer>(m));
   objects.push_back(obj);
   */


   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();
      
      camera.transform.position = Ignition::Vector3(-3, 0, -1);

      for (int i = 0; i < objects.size(); i++)
      {

         if (objects[i].enabled)
            objects[i].Update();
      }
      gui.Style(); 
      gui.NewFrame();
       
      if (objects.size() > 0)
      {
         gui.Inspector(&objects[0]);
      } else {
         gui.Inspector(nullptr);
      }
      

      gui.DebugMenu();
      gui.MenuBar(&objects);
      //ImGui::ShowDemoWindow();
      gui.EndFrame();
      
      camera.EndRender();
   }
   
   gui.Shutdown();
   window.Shutdown();
}
