#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "camera.hpp"
#include "modules/script.hpp"

namespace Ignition::Scripting::Lua {
   extern lua_State* state;
   
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
   
   void LoadIgnitionLibrary();
   void RegisterFunctionsAndClasses();

   std::vector<LuaData> GetScriptVariables(Script*);
   void RunScript(Script*);

   namespace Functions {
      void Shutdown();
      void Reboot();

      Ignition::Camera* GetCamera();
      
      int CreateObject();
   }
}
