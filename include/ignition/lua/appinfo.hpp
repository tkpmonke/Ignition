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
}
