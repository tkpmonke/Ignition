#include "modules/script.hpp"
#include "utils/io.hpp"
#include <filesystem>
namespace Ignition {
   void Script::Start() {
      if (std::filesystem::exists(path) && !std::filesystem::is_directory(path)) {
         state = luaL_newstate();
         LoadIgnitionLibrary(state);

         luabridge::setGlobal(state, this->transform, "transform");
         
         luaL_dofile(state, path.data());

         if (lua_gettop(state)>0) {
            if (lua_istable(state, -1)) {
               module = luabridge::LuaRef::fromStack(state, -1);
               lua_pop(state, 1);
               if (module["Start"].isFunction()) 
                  module["Start"]();
               init = true;
            } else {
               Ignition::IO::Error(path + " does not return a table");
               return;
            }
         }
         
      }
   }

   void Script::Update() {
      if (init) {
         luabridge::setGlobal(state, glfwGetTime(), "time");
         module["Update"]();
      } else if (std::filesystem::exists(path) && !std::filesystem::is_directory(path)) {
         Start();
      }
   }

   void Script::Shutdown() {
      module["Shutdown"]();

      lua_close(state);
      state = nullptr;
   }

   void Script::GetLuaScriptVariables() {

      std::vector<LuaData> o;
      int a = lua_gettop(state);
      lua_pushnil(state);
   
      while (lua_next(state, a-1) != 0) {
         const char* name;
         name = lua_tostring(state, -2);

         if (lua_isboolean(state, -1)) {
            o.push_back({
               .data = {.b = (bool)lua_toboolean(state, -1)}, 
               .type = LuaData::Boolean, 
               .name = name
            });
         } else if (lua_isnumber(state, -1)) {
            o.push_back({
               .data = {.f = lua_tonumber(state, -1)}, 
               .type = LuaData::Float, 
               .name = name
            });
         } else if (lua_isstring(state, -1)) {
            o.push_back({
               .data = {.s = lua_tostring(state, -1)}, 
               .type = LuaData::String, 
               .name = name
            });
         }
      }

      this->variables = o;
   }
}
