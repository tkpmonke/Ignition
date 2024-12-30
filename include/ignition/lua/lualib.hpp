#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "camera.hpp"

namespace Ignition::Scripting::Lua {
   class LuaData { 
   public:
      float f = 0;
      std::string s = "";
      bool b = false;

      enum Type {
         Float,
         String,
         Boolean
      } type;

      std::string name;
   };
   
   void LoadIgnitionLibrary(lua_State* state);
   void RegisterFunctionsAndClasses(lua_State* state);


   namespace Functions {
      void Shutdown();
      void Reboot();

      Ignition::Camera* GetCamera();
      
      int CreateObject();
   }
}
