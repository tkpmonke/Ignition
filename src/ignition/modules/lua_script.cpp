#include "modules/script.hpp"
#include "utils/io.hpp"
#include "utils/files.hpp"

#include <filesystem>
namespace Ignition {
   bool allowScripting = true;
   void Script::Start() {
      if (std::filesystem::exists(Ignition::IO::GetProjectHome()+path) && !std::filesystem::is_directory(Ignition::IO::GetProjectHome()+path)) {
         state = luaL_newstate();
         LoadIgnitionLibrary(state);

         luabridge::setGlobal(state, this->transform, "transform");
         luabridge::setGlobal(state, this->object, "object");
         
         if (luaL_dofile(state, (Ignition::IO::GetProjectHome()+path).data()) != LUA_OK) {
            Ignition::IO::Error("Error Loading Lua Script");
            Ignition::IO::Error(lua_tostring(state, -1));
            lua_pop(state, 1);
            return;
         }

         if (lua_gettop(state)>0) {
            if (lua_istable(state, -1)) {
               module = luabridge::LuaRef::fromStack(state, -1);
               lua_pop(state, 1);
               if (module["Start"].isFunction() && allowScripting) 
                  module["Start"]();
               init = true;
            } else {
               return;
            }
         }
         
      }
   }

   float ptime = 0;
   void Script::Update() {
      float time = glfwGetTime();
      float dt = time-ptime;
      ptime = time;

      if (init) {
         luabridge::setGlobal(state, time, "ig_time");
         luabridge::setGlobal(state, dt, "ig_deltaTime");
         luabridge::setGlobal(state, this->transform, "transform");
         luabridge::setGlobal(state, this->object, "object");
         if (module["Update"].isFunction()) { 
            module["Update"]();
         }
      } else if (std::filesystem::exists(Ignition::IO::GetProjectHome()+path)
         && !std::filesystem::is_directory(Ignition::IO::GetProjectHome()+path)) {
         Start();
      }
   }

   void Script::Shutdown() {
      if (module["Shutdown"].isFunction()) 
         module["Shutdown"]();

      lua_close(state);
      state = nullptr;
   }

   void Script::Serialize() {
      Ignition::IO::WriteString(path);
   }

   void Script::Deserialize() {
      this->path = Ignition::IO::ReadString();
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
