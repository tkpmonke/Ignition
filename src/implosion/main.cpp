#include "window.hpp"
#include "camera.hpp"
#include "gui/gui.hpp"
#include "input/camera_movement.hpp"
#include "types/texture.hpp"
#include "utils/files.hpp"

#include <iostream>

using namespace Ignition::Rendering;

int main() 
{
   Ignition::Window window = Ignition::Window(1920, 1080, "Implosion");
   Ignition::Camera camera = Ignition::Camera(&window);
   camera.fov = 75;
   camera.clipping_planes.min = 0.1f;
   camera.clipping_planes.max = 100.f;
   camera.transform.position = Ignition::Vector3(-5,1,0);
   camera.MakeMainCamera();

   std::vector<Ignition::Object> objects;

   Implosion::GUI gui = Implosion::GUI(window, &camera);
   gui.InitGrid();
   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();
       
      cameraMovement(&window, &camera);
      
      gui.RenderGrid();

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

      gui.Preferences();

      gui.MenuBar(&objects);
      //ImGui::ShowDemoWindow();
      gui.EndFrame();
      
      camera.EndRender();
   }
   
   gui.Shutdown();
   window.Shutdown();
}
