#include "modules/script.hpp"
#include "lua/lualib.hpp"

#include "window.hpp"
#include "camera.hpp"
#include "project.hpp"

#include "utils/files.hpp"

#include <iostream>

int main() {
   Ignition::Scripting::Lua::LoadIgnitionLibrary();

   Ignition::IO::SetProjectHome("/home/turdle/Projects/implosion-test/game");

   bool applicationOpen = true;
   while (applicationOpen) {
      Ignition::Window window = Ignition::Window(1920, 1080, "Implosion", &applicationOpen);
      Ignition::Camera camera = Ignition::Camera(&window);
      camera.fov = 75;
      camera.clipping_planes.min = 0.1f;
      camera.clipping_planes.max = 100.f;
      camera.transform.position.y = 5;
      camera.MakeMainCamera();

      Ignition::project.LoadProject(&window);
      
      if (luaL_dofile(Ignition::Scripting::Lua::state, (Ignition::IO::GetProjectHome()+"/settings/app.lua").data()) != LUA_OK) {
        std::cerr << "Error: " << lua_tostring(Ignition::Scripting::Lua::state, -1) << std::endl;
        lua_close(Ignition::Scripting::Lua::state);
        return -1;
      }
         
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
