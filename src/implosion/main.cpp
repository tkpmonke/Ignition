#include "window.hpp"
#include "camera.hpp"
#include "gui/gui.hpp"
#include "input/camera_movement.hpp"
#include "serialization/saving.hpp"
#include "scene.hpp"
#include "project.hpp"
#include "utils/model_loader.hpp"
#include "input/commands.hpp"
#include "sprites/skybox_top.h"
#include "sprites/skybox_sides.h"

#include <iostream>
#include <string.h>

const char* implosionHelp = 
"Implosion Editor\n"
"\n"
"-h  / --help     Prints This Help Message\n"
"-f  / --project  Specify Project Directory\n"
"-i               Prints Blank Statement (used by hub) \n"
"-t  / --tests    Runs Unit Tests\n"
"-hl / --headless Runs Headless\n";

using namespace Ignition::Rendering;

int main(int argc, char** argv) 
{


   for (int i = 0; i < argc; ++i)
   {
      if (strcmp(argv[i], "-f") == 0
       || strcmp(argv[i], "--project") == 0)
      {
         Ignition::IO::SetProjectHome(argv[++i]);
      }
      if (strcmp(argv[i], "-i") == 0)
      {
         std::cout << "Ignition Engine Is Installed\n";
         return 0;
      }
      if (strcmp(argv[i], "-h") == 0
       || strcmp(argv[i], "--help") == 0)
      {
         std::cout << implosionHelp << "\n";
         return 0;
      }
   }
   
   if (Ignition::IO::GetProjectHome() == "")
   {
      std::cerr << "No Project Provided\n";
      return -1;
   }

   bool applicationOpen = true;
   while (applicationOpen) {
      Ignition::Window window = Ignition::Window(1920, 1080, "Implosion", &applicationOpen);
      Ignition::Camera camera = Ignition::Camera(&window);
      camera.fov = 75;
      camera.clipping_planes.min = 0.1f;
      camera.clipping_planes.max = 100.f;
      camera.transform.position = Ignition::Vector3(-5,1,0);
      camera.MakeMainCamera();

      Ignition::project.LoadProject(&window);

      Implosion::GUI gui = Implosion::GUI(&window, &camera);
      ReadPreferences(&gui);

      std::vector<const char*> skyboxImages = 
      {
               skybox_sides.pixel_data,
               skybox_sides.pixel_data,
                 skybox_top.pixel_data,
                 skybox_top.pixel_data,
               skybox_sides.pixel_data,
               skybox_sides.pixel_data,
      };
      Ignition::scene.skybox = Ignition::Skybox(skyboxImages, 64, 64, 3, "IMPLOSION_SKYBOX");

      gui.InitGrid();

      pp_manager = PPManager(&window);

      SetCommandCallback(window, &gui);

      gui.Style();
         
      while (window.IsOpen())
      {
         window.Update();
         window.Bind();
         camera.BeginRender();
          
         cameraMovement(&window, &camera);
         
         Ignition::scene.skybox.Render();

         gui.RenderGrid();

         Ignition::scene.Update();
         
         gui.NewFrame();

         gui.SceneHierarchy();

         gui.FileExplorer();

         gui.DebugMenu();

         gui.Preferences();

         gui.MenuBar();
         //ImGui::ShowDemoWindow();
         
         gui.SceneView();

         gui.RenderPopups();

         gui.PostProcessManagerUI();

         gui.Inspector();

         camera.EndRender(true);
         gui.EndFrame();
         camera.EndGUI();
      }

      WritePreferences(); 
      Ignition::scene.WriteSceneToDisk();

      Ignition::scene.Shutdown();
      gui.Shutdown();
      window.Shutdown();
   }
}
