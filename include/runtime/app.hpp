#include <lua.hpp>

struct appinfo {
   const char* appName;
   const char* version;
};
extern appinfo appInfo;

void loadAppInfo(lua_State* L);
