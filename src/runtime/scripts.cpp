#include "scripts.hpp"
#include "GLFW/glfw3.h"

#define LUA_SET_GLOBAL(L, x, v) \
   lua_pushnumber(L, v);\
   lua_setglobal(L, x);

void callUpdate(lua_State* L) {
   lua_getglobal(L, "update");
   if (lua_isfunction(L, -1)) {
      lua_call(L, 0, 0);
   } else {
      lua_pop(L, 1);
   }
}

void setPerFrameVariables(lua_State* L) {
  LUA_SET_GLOBAL(L, "IGtime", glfwGetTime()); 
}
