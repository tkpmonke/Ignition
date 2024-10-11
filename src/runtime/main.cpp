#include "app.hpp"
#include "lib.hpp"
#include "scripts.hpp"

#include "window.hpp"
#include "camera.hpp"
#include "project.hpp"

#include "utils/files.hpp"

int main() {
   lua_State* L = luaL_newstate();
   luaL_openlibs(L);
   open_lib(L);

   //std::cout << "App Name : " << appInfo.appName << "\n";
   //std::cout << "Version : " << appInfo.version << "\n";

   Ignition::IO::SetProjectHome("/home/turdle/Projects/implosion-test/game");

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
         if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) loadAppInfo(L);
         
         window.Update();
         window.Bind();
         camera.BeginRender();
          
         Ignition::scene.Update();

         setPerFrameVariables(L);
         callUpdate(L);
         
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


   lua_close(L);
}
