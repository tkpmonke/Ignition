#pragma once

#include "module.hpp"
#include "lua/lualib.hpp"

#include <vector>

using namespace Ignition::Scripting::Lua;

namespace Ignition {
   class Script : public Module {
   public:
      CREATE_MODULE("Script")

      // defaults to lua
      enum { 
         Lua = 0,
// THESE ARE PLANNED LANGUAGES
//         CS = 1,
//         Python = 2
      } language = Lua;

      std::string path;
      std::vector<LuaData> variables;

      Script() : state(luaL_newstate()), module(state) {}

      void Start() override;
      void Update() override;
      void Shutdown() override;

   private:
      void GetLuaScriptVariables();
      lua_State* state;
      bool init = false;
      luabridge::LuaRef module;
   };
}
