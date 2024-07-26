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

   Implosion::GUI gui = Implosion::GUI();

   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();
       
      gui.NewFrame();
      
      gui.DebugMenu();

      gui.EndFrame();

      camera.EndRender();
   }
   
   window.Shutdown();
}
