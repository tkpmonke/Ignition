#include "window.hpp"
#include "camera.hpp"
#include "project.hpp"

#include "utils/files.hpp"

#include <filesystem>

int main() {
   std::filesystem::path exePath = Ignition::IO::ReadTextFile("/proc/self/exe");
   Ignition::IO::SetProjectHome(exePath.parent_path().string());


   bool applicationOpen = true;
   while (applicationOpen) {
      Ignition::Window window = Ignition::Window(1920, 1080, "Implosion", &applicationOpen);
      Ignition::Camera camera = Ignition::Camera(&window);
      camera.fov = 75;
      camera.clipping_planes.min = 0.1f;
      camera.clipping_planes.max = 100.f;

      camera.MakeMainCamera();

      Ignition::project.LoadProject(&window);
         
      int i = 0;
      while (window.IsOpen())
      {
         window.Update();
         window.Bind();
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
      window.Shutdown();
   }
}
