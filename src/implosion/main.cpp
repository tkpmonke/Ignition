#include "window.hpp"
#include "camera.hpp"
#include "gui/gui.hpp"
#include "input/camera_movement.hpp"
#include "serialization/saving.hpp"
#include "scene.hpp"

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

   Implosion::GUI gui = Implosion::GUI(window, &camera);
   ReadPreferences(&gui);
   Ignition::Scene scene;
   Ignition::scene = scene;
   gui.InitGrid();
   gui.Style(); 
   while (window.IsOpen())
   {
      window.Update();
      camera.BeginRender();
       
      cameraMovement(&window, &camera);
      
      gui.RenderGrid();

      Ignition::scene.Update();
      gui.NewFrame();
      if (Ignition::scene.GetObjects()->size() > 0)
      {
         gui.Inspector(&Ignition::scene.GetObjects()->at(0));
      } else {
         gui.Inspector(nullptr);
      }

      gui.DebugMenu();

      gui.Preferences();

      gui.MenuBar();
      //ImGui::ShowDemoWindow();
      gui.EndFrame();
      camera.EndRender();
   }
   
   gui.Shutdown();
   window.Shutdown();
}
