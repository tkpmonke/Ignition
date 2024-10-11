#include <lua.hpp>

void open_lib(lua_State*);

// some generic stuff
int poweroff(lua_State*);
int reboot(lua_State*);

int get_camera(lua_State*);

int create_object(lua_State*);
