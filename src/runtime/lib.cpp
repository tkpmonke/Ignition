#include "lib.hpp"
#include "camera.hpp"

/* UTILITY MACROS */
#define LUA_CFLOAT_VARIABLE(L, x, n) \
lua_pushcfunction(L, [](lua_State* L) -> int {     \
   int nargs = lua_gettop(L); \
   if (nargs == 0) {          \
      lua_pushnumber(L, x);   \
      return 1;               \
   } else if (nargs == 1) {   \
      x = luaL_checknumber(L, 1);\
      return 0;               \
   }                          \
   return luaL_error(L,       \
  "Invalid number of arguments");\
}); \
lua_setfield(L, -2, n);

#define LUA_OBJECT_VARIABLES(L, x) \
   lua_createtable(L, 0, 1);        \
   x(L);                             \
   lua_setfield(L, -2, "__index");  \
   lua_createtable(L, 0, 1);        \
   x(L);                             \
   lua_setfield(L, -2, "__newindex");\

/* ENGINE STATUS */
int poweroff(lua_State* L) {
   Ignition::MainCamera::camera->window->Close();
   return 0;
}

int reboot(lua_State* L) { 
   Ignition::MainCamera::camera->window->Restart();
   return 0;
}

/* CAMERA */
int get_camera(lua_State* L) {
   Ignition::Camera** cam = (Ignition::Camera**)lua_newuserdata(L, sizeof(Ignition::Camera*));
   *cam = Ignition::MainCamera::camera;

   luaL_getmetatable(L, "IGCameraTable");
   lua_setmetatable(L, -2);
   return 1;
}


void registerCamera(lua_State* L) {
   luaL_newmetatable(L, "IGCameraTable");
   LUA_OBJECT_VARIABLES(L, [&](lua_State*L ) {
      LUA_CFLOAT_VARIABLE(L, Ignition::MainCamera::camera->fov, "fov");
      LUA_CFLOAT_VARIABLE(L, Ignition::MainCamera::camera->clipping_planes.min, "min");
      LUA_CFLOAT_VARIABLE(L, Ignition::MainCamera::camera->clipping_planes.max, "max");
      LUA_CFLOAT_VARIABLE(L, Ignition::MainCamera::camera->size.x, "width");
      LUA_CFLOAT_VARIABLE(L, Ignition::MainCamera::camera->size.y, "height");
   });
   lua_pushstring(L, "IGCameraTable");
   lua_setfield(L, -2, "__metatable");
   
   lua_pop(L, 1);
}

/* OPEN LIBRARY FOR LUA */
void open_lib(lua_State* L) {
   /* CLASSES */
   registerCamera(L);

   /* FUNCTIONS */
   lua_register(L, "get_camera", get_camera);
   lua_register(L, "poweroff", poweroff);
   lua_register(L, "reboot", reboot);
}
