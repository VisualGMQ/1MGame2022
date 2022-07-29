#include "hazel/hazel.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int main() {
    lua_State* L = luaL_newstate();
    if (!L) {
        printf("lua init failed\n");
        return 1;
    }
    // luaL_openlibs(L);

    if (luaL_loadfile(L, "main.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("main.lua load and execute failed\n");
        return 1;
    }

    lua_close(L);
    return 0;
}