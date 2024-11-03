#pragma once

#include "module.hpp"


#include <vector>

namespace Ignition {
   class Script : Module {
   public:
      CREATE_MODULE("Script")

      // defaults to lua
      enum { 
         Lua = 0,
// THESE ARE PLANNED LANGUAGES
//         CS = 1,
//         Python = 2
      } language = Lua;

      const char* script = nullptr;
      char* scriptLocation = nullptr;

      Script();

      void Start() override;
      void Update() override;
      void Shutdown() override;
      void Serialize() override;
      void Deserialize() override; 
   private:
   };
}
