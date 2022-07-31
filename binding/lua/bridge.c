#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaconf.h"

#include "hazel/hazel.h"
#include "hazel/renderer.h"

static int LuaBridge_RenderSetClearColor(lua_State* L) {
    float r = luaL_checknumber(L, 1);
    float g = luaL_checknumber(L, 2);
    float b = luaL_checknumber(L, 3);
    float a = luaL_checknumber(L, 4);
    Hazel_RenderSetClearColor(r, g, b, a);
    return 0;
}

static int LuaBridge_SayHello(lua_State* L) {
    printf("Hello Hazel\n");
    return 0;
}

static int LuaBridge_RenderClear(lua_State* L) {
    Hazel_RenderClear();
    return 0;
}

static int LuaBridge_RenderSetDrawColor(lua_State* L) {
    float r = luaL_checknumber(L, 1);
    float g = luaL_checknumber(L, 2);
    float b = luaL_checknumber(L, 3);
    float a = luaL_checknumber(L, 4);
    Hazel_RenderSetDrawColor(r, g, b, a);
    return 0;
}

static int LuaBridge_RenderDrawLine(lua_State* L) {
    float x1 = luaL_checknumber(L, 1);
    float y1 = luaL_checknumber(L, 2);
    float x2 = luaL_checknumber(L, 3);
    float y2 = luaL_checknumber(L, 4);
    Hazel_RenderDrawLine(x1, y1, x2, y2);
    return 0;
}

static int LuaBridge_RenderDrawRect(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float w = luaL_checknumber(L, 3);
    float h = luaL_checknumber(L, 4);
    Hazel_RenderDrawRect(x, y, w, h);
    return 0;
}

static int LuaBridge_RenderFillRect(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float w = luaL_checknumber(L, 3);
    float h = luaL_checknumber(L, 4);
    Hazel_RenderFillRect(x, y, w, h);
    return 0;
}

static int LuaBridge_LoadTexture(lua_State* L) {
    const char* filename = luaL_checkstring(L, 1);
    Hazel_Texture* texture = Hazel_LoadTexture(filename);
    lua_pushlightuserdata(L, texture);
    return 1;
}

static int LuaBridge_DestroyTexture(lua_State* L) {
    Hazel_Texture* texture = (Hazel_Texture*)lua_topointer(L, 1);
    Hazel_DestroyTexture(texture);
    return 0;
}

static int LuaBridge_TextureGetSize(lua_State* L) {
    Hazel_Texture* texture = (Hazel_Texture*)lua_topointer(L, 1);
    int w, h;
    Hazel_TextureGetSize(texture, &w, &h);
    lua_pushnumber(L, w);
    lua_pushnumber(L, h);
    return 2;
}

static int LuaBridge_RenderDrawTexture(lua_State* L) {
    Hazel_Texture* texture = (Hazel_Texture*)lua_topointer(L, 1);
    int rx = luaL_checknumber(L, 2);
    int ry = luaL_checknumber(L, 3);
    int rw = luaL_checknumber(L, 4);
    int rh = luaL_checknumber(L, 5);
    int dx = luaL_checknumber(L, 6);
    int dy = luaL_checknumber(L, 7);
    int dw = luaL_checknumber(L, 8);
    int dh = luaL_checknumber(L, 9);
    Hazel_RenderFlip flip = luaL_checknumber(L, 10);
    Hazel_RenderDrawTexture(texture, rx, ry, rw, rh, dx, dy, dw, dh, flip);
    return 0;
}

static int LuaBridge_GetCanvaSize(lua_State* L) {
    int w, h;
    Hazel_GetCanvaSize(&w, &h);
    lua_pushnumber(L, w);
    lua_pushnumber(L, h);
    return 2;
}

static int LuaBridge_WindowSetIcon(lua_State* L) {
    const char* filename = luaL_checkstring(L, 1);
    Hazel_WindowSetIcon(filename);
    return 0;
}

static int LuaBridge_GetKeyState(lua_State* L) {
    int key = luaL_checkinteger(L, 1);
    lua_pushnumber(L, Hazel_GetKeyState(key));
    return 1;
}

static int LuaBridge_GetMousePos(lua_State* L) {
    double x, y;
    Hazel_GetMousePos(&x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}

static int LuaBridge_GetMouseButtonState(lua_State* L) {
    int btn = luaL_checknumber(L, 1);
    lua_pushnumber(L, Hazel_GetMouseBtnState(btn));
    return 1;
}

static int LuaBridge_GetTime(lua_State* L) {
    lua_pushnumber(L, Hazel_GetTime());
    return 1;
}

static int LuaBridge_GetWindowSize(lua_State* L) {
    int w, h;
    Hazel_WindowGetSize(&w, &h);
    lua_pushnumber(L, w);
    lua_pushnumber(L, h);
    return 2;
}

static int LuaBridge_HideCursor() {
    Hazel_HideCursor();
    return 0;
}

static int LuaBridge_ShowCursor() {
    Hazel_ShowCursor();
    return 0;
}

static const struct luaL_Reg libhazel[] = {
    {"RenderSetClearColor", LuaBridge_RenderSetClearColor},
    {"RenderSetDrawColor", LuaBridge_RenderSetDrawColor},
    {"RenderClear", LuaBridge_RenderClear},
    {"RenderDrawLine", LuaBridge_RenderDrawLine},
    {"RenderDrawRect", LuaBridge_RenderDrawRect},
    {"RenderFillRect", LuaBridge_RenderFillRect},
    {"RenderDrawTexture", LuaBridge_RenderDrawTexture},
    {"TextureGetSize", LuaBridge_TextureGetSize},
    {"LoadTexture", LuaBridge_LoadTexture},
    {"DestroyTexture", LuaBridge_DestroyTexture},
    {"GetCanvaSize", LuaBridge_GetCanvaSize},
    {"WindowSetIcon", LuaBridge_WindowSetIcon},
    {"GetTime", LuaBridge_GetTime},
    {"GetWindowSize", LuaBridge_GetWindowSize},
    {"GetMousePos", LuaBridge_GetMousePos},
    {"GetMouseButtonState", LuaBridge_GetMouseButtonState},
    {"GetKeyState", LuaBridge_GetKeyState},
    {"HideCursor", LuaBridge_HideCursor},
    {"ShowCursor", LuaBridge_ShowCursor},
    {"SayHello", LuaBridge_SayHello},
    {NULL, NULL},
};

DLLEXPORT int luaopen_libhazel(lua_State* L) {
    luaL_newlib(L, libhazel);
    return 1;
}
