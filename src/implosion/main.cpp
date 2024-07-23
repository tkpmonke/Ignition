#include "window.hpp"
#include "camera.hpp"

#include <iostream>

int main() 
{
   Ignition::Window window = Ignition::Window(700, 700, "Implosion");
   Ignition::Camera camera = Ignition::Camera(&window);
   camera.fov = 75;
   camera.clipping_planes.min = 0.1f;
   camera.clipping_planes.max = 100.f;
   camera.MakeMainCamera();

   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();

      camera.EndRender();
   }
   
   window.Shutdown();
}
