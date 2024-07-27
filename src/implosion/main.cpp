#include "window.hpp"
#include "camera.hpp"

#include "gui/gui.hpp"

#include <iostream>

int main() 
{
   Ignition::Window window = Ignition::Window(700, 700, "Implosion");
   Ignition::Camera camera = Ignition::Camera(&window);
   camera.fov = 75;
   camera.clipping_planes.min = 0.1f;
   camera.clipping_planes.max = 100.f;
   camera.MakeMainCamera();

   std::vector<Ignition::Object> objects;

   Implosion::GUI gui = Implosion::GUI(window);

   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();
       
      for (Ignition::Object obj : objects)
      {
         obj.Update();
      }

      gui.NewFrame();
      
      gui.DebugMenu();
      gui.MenuBar(&objects);

      gui.EndFrame();
      
      camera.EndRender();
   }
   
   gui.Shutdown();
   window.Shutdown();
}
