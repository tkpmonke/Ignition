#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "camera.hpp"

namespace Ignition::Scripting::Lua {
   struct LuaData { 
      union {
         float f;
         const char* s;
         bool b;
      } data;

      enum {
         Float,
         String,
         Boolean
      } type;

      const char* name;
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
