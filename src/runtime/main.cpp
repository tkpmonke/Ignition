#include "window.hpp"
#include "camera.hpp"
#include "project.hpp"

#include "utils/files.hpp"
#include "vels/vels.hpp"

#include <filesystem>
#include "utils/io.hpp"

bool Ignition::IO::editor = false;
int main() {
#ifdef EDITOR 
   Ignition::IO::DebugPrint("uh this shouldn't be an editor");
#endif
   std::filesystem::path exePath = Ignition::IO::ReadTextFile("/proc/self/exe");
   Ignition::IO::SetProjectHome(exePath.parent_path().string());


   bool applicationOpen = true;
   while (applicationOpen) {
      Ignition::Window window = Ignition::Window(1920, 1080, exePath.filename().string().data(), &applicationOpen);
      Ignition::Camera camera = Ignition::Camera(&window);
      camera.fov = 75;
      camera.clipping_planes.min = 0.1f;
      camera.clipping_planes.max = 100.f;

      camera.MakeMainCamera();

      vels::World physicsWorld = vels::World();

      Ignition::project.LoadProject(&window);
         
      while (window.IsOpen())
      {
         window.Update();
         window.Bind();
         physicsWorld.Update();
         camera.BeginRender();
          
         Ignition::scene.Update();

         camera.EndRender(false);
         int w,h;
         glfwGetWindowSize(window, &w, &h);
         camera.size.x = w;
         camera.size.y = h;
         window.Resize(w,h);
      }

      Ignition::scene.Shutdown();
      physicsWorld.Shutdown();
      window.Shutdown();
   }
}
