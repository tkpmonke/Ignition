#include "window.hpp"
#include "camera.hpp"
#include "gui/gui.hpp"
#include "input/camera_movement.hpp"
#include "serialization/saving.hpp"
#include "scene.hpp"
#include "project.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "utils/unlit_shader.hpp"
#include "shapes/cube.hpp"
#include "textures/grid.hpp"
#include "utils/model_loader.hpp"
#include "utils/postprocess.hpp"
#include "utils/postprocessing_shaders.hpp"

#include <iostream>
#include <string.h>

using namespace Ignition::Rendering;

int main(int argc, char** argv) 
{

   for (int i = 0; i < argc; ++i)
   {
      if (strcmp(argv[i], "-f") == 0)
      {
         FS::SetProjectHome(argv[++i]);
      }
      if (strcmp(argv[i], "-i") == 0)
      {
         std::cout << "Ignition Engine Is Installed\n";
         return 0;
      }
   }
   
   if (FS::GetProjectHome() == "")
   {
      std::cerr << "No Project Provided\n";
      return -1;
   }

   Ignition::Window window = Ignition::Window(1920, 1080, "Implosion");
   Ignition::Camera camera = Ignition::Camera(&window);
   camera.fov = 75;
   camera.clipping_planes.min = 0.1f;
   camera.clipping_planes.max = 100.f;
   camera.transform.position = Ignition::Vector3(-5,1,0);
   camera.MakeMainCamera();
   

   Implosion::GUI gui = Implosion::GUI(&window, &camera);
   ReadPreferences(&gui);
   Ignition::project = Ignition::Project(0);

   gui.InitGrid();
   gui.Style(); 

   pp_manager = PPManager(&window);

   while (window.IsOpen())
   {
      window.Update();
      window.Bind();
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
      
      gui.SceneView();

      gui.PostProcessManagerUI();

      camera.EndRender(true);
      gui.EndFrame();
      camera.EndGUI();
   }
   WritePreferences(); 
   Ignition::scene.WriteSceneToDisk();
   gui.Shutdown();
   window.Shutdown();
}
