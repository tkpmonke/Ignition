#include "app.hpp"

#include <filesystem>
#include <string>
#include <iostream>

appinfo appInfo;

void loadAppInfo(lua_State* L) {
   //std::filesystem::path path = std::filesystem::current_path();
   std::filesystem::path path = "/home/turdle/Projects/implosion-test/game";
   if (luaL_dofile(L, (path.string()+"/settings/app.lua").data())) {
      std::cerr << "ERROR IN LUA FILE : " << lua_tostring(L, -1) << "\n";
      exit(-1);
   }

   if (!lua_istable(L, -1)) {
      std::cerr << "app.lua MUST RETURN A TABLE\n";
      exit(-2);
   }

   lua_getfield(L, -1, "appName");
   appInfo.appName = lua_tostring(L, -1);
   lua_pop(L, 1);

   lua_getfield(L, -1, "version");
   appInfo.version = lua_tostring(L, -1);
   lua_pop(L, 2);

}
