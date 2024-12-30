#include "lualib.hpp"
#include "utils/io.hpp"
#include "utils/files.hpp"
#include <string>

namespace Ignition::Scripting::Lua {

   inline struct AppInfo {
      std::string appName;
      std::string version;
      std::string devName;
      std::string graphicsAPI;
   } appInfo;

   inline struct CameraInfo {
      Vector3 position, rotation;
      float fov, min, max;
   } cameraInfo;

   inline void LoadAppInfo() {
      lua_State* L = luaL_newstate();
      luaL_openlibs(L);
      luaL_dofile(L, (Ignition::IO::GetProjectHome()+"/settings/app.lua").data());

      if (lua_gettop(L) > 0) {
         try {
            luabridge::LuaRef M = luabridge::LuaRef::fromStack(L, -1);
            if (M.isTable()) {
               appInfo.appName = M["appName"].cast<std::string>();
               appInfo.version = M["version"].cast<std::string>();
               appInfo.devName = M["devName"].cast<std::string>();
               appInfo.graphicsAPI = M["graphicsAPI"].cast<std::string>();
            }
         } catch (const luabridge::LuaException& e) {
            Ignition::IO::Error("Error reading app.lua table : " + (std::string)e.what());
         }
      } else {
         Ignition::IO::Error("app.lua did not return a table");
      }

      lua_close(L);
   }
   
   inline void LoadCameraInfo() {
      lua_State* L = luaL_newstate();
      LoadIgnitionLibrary(L);
      luaL_dofile(L, (Ignition::IO::GetProjectHome()+"/settings/camera.lua").data());

      if (lua_gettop(L) > 0) {
         try {
            luabridge::LuaRef M = luabridge::LuaRef::fromStack(L, -1);
            if (M.isTable()) {
               cameraInfo.position = M["position"].cast<Ignition::Vector3>();
               cameraInfo.rotation = M["rotation"].cast<Ignition::Vector3>();
               cameraInfo.fov = M["fov"].cast<float>();
               cameraInfo.min = M["near"].cast<float>();
               cameraInfo.max = M["far"].cast<float>();
            }
         } catch (const luabridge::LuaException& e) {
            Ignition::IO::Error("Error reading camera.lua table : " + (std::string)e.what());
         }
      } else {
         Ignition::IO::Error("camera.lua did not return a table");
      }

      lua_close(L);
   }
}
