#include "luaT.h"

extern void lab_Floatinit(lua_State *L);
extern void lab_Doubleinit(lua_State *L);
extern void lab_utils_init(lua_State *L);

DLL_EXPORT int luaopen_liblab(lua_State *L)
{
  lua_newtable(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, LUA_GLOBALSINDEX, "lab");

  lab_Floatinit(L);
  lab_Doubleinit(L);
  lab_utils_init(L);

  return 1;
}
