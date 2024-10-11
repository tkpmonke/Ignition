#define __INCLUDE_LUA_RUNTIME
#ifdef __INCLUDE_LUA_RUNTIME
#include "lua.hpp"
#endif
#include "module.hpp"

#include <vector>

namespace Ignition {

   struct script_data_types { 
      union {
         float f;
         const char* s;
         int b;
      } data;

      enum {
         Float,
         String,
         Boolean
      } type;

      const char* name;
   };

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

      const char* script;
      char* scriptLocation = nullptr;

      Script();

      void Start() override;
      void Update() override;
      void Shutdown() override;
      void Serialize() override;
      void Deserialize() override; 
   private:
#ifdef __INCLUDE_LUA_RUNTIME
      void lua_run();
      std::vector<script_data_types> lua_getvariables();
      void lua_trycallfunction(const char*);
      lua_State* state;
#endif
   };
}
