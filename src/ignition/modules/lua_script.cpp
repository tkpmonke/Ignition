#include "modules/script.hpp"

#ifdef __INCLUDE_LUA_RUNTIME
namespace Ignition {
   /*
   std::vector<script_data_types> lua_getvariables() {
      std::vector<script_data_types> o;
      int a = lua_gettop(this->state);
      lua_pushnil(this->state);
   
      while (lua_next(this->state, a-1) != 0) {
         const char* name;
         name = lua_tostring(this->state, -2);

         if (lua_isboolean(this->state, -1)) {
            o.push_back({
               .data = {.b = lua_toboolean(this->state, -1)}, 
               .type = script_data_types::Boolean, 
               .name = name
            });
         } else if (lua_isnumber(this->state, -1)) {
            o.push_back({
               .data = {.f = lua_tonumber(this->state, -1)}, 
               .type = script_data_types::Float, 
               .name = name
            });
         } else if (lua_isstring(this->state, -1)) {
            o.push_back({
               .data = {.s = lua_tostring(this->state, -1)}, 
               .type = script_data_types::String, 
               .name = name
            });
         }
      }

      return o;
   }
   */
}
#endif
