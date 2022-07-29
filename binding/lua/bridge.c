#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaconf.h"

#include "hazel/hazel.h"

static int LuaBridge_RunExampleWindow(lua_State* L) {
    Hazel_RunExampleWindow();
    return 0;
}

static const struct luaL_Reg hazel[] = {
    {"RunExampleWindow", LuaBridge_RunExampleWindow},
    {NULL, NULL},
};

DLLEXPORT int luaopen_hazel(lua_State* L) {
    luaL_newlib(L, hazel);
    return 1;
}