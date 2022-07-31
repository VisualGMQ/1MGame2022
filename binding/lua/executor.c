#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "hazel/hazel.h"

typedef struct {
    int WindowWidth;
    int WindowHeight;
    char WindowTitle[1024];
    char LuaEntryFile[1024];
} Config;

int getNumber(lua_State* L, const char* name, int* num) {
    int isnum;
    lua_getglobal(L, name);
    *num = (int)lua_tointegerx(L, -1, &isnum);
    if (!isnum) {
        printf("Please check your %s field", name);
        return 0;
    }
    return 1;
}

int getString(lua_State* L, const char* name, char* str) {
    lua_getglobal(L, name);
    const char* s = lua_tostring(L, -1);
    if (!s) {
        printf("Please check your %s filed", name);
        lua_close(L);
        return 0;
    }
    strcpy(str, s);
    return 1;
}

const char* LuaConfigFileTemplate =
"WindowWidth = 800\n"
"WindowHeight = 600\n"
"Title = \"LuaTinyEngine\"\n"
"EntryFile = \"main.lua\"";

void createLuaConfigFile() {
    FILE* file = fopen("config.lua", "w+");
    if (!file) {
        printf("can't open file config.lua");
        return ;
    }
    fputs(LuaConfigFileTemplate, file);
    fclose(file);
}

const char* LuaEntryFileTemplate =
"local hazel = require \"hazel\"\n"
"\n"
"function GameStart()\n"
"    hazel.SayHello()\n"
"end\n"
"\n"
"function GameLoop()\n"
"end\n"
"\n"
"function GameQuit()\n"
"end";

void createLuaEntryFile() {
    FILE* file = fopen("main.lua", "w+");
    if (!file) {
        printf("can't open file main.lua");
        return ;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size <= 0) {
        fputs(LuaEntryFileTemplate, file);
        fclose(file);
    }
}

int ReadConfig(Config* config) {
    if (!config) {
        printf("please give me a valid config");
        return 0;
    }

    lua_State* L = luaL_newstate();
    if (!L) {
        printf("Fatal Error, create lua stack failed");
        return 1;
    }
    if (luaL_loadfile(L, "config.lua") != 0) {
        printf("You don't have a config.lua, create one for you.Modify it to your config.");
        createLuaConfigFile();
        goto ERR_HANDLE;
    }
    if (lua_pcall(L, 0, 0, 0) != 0) {
        printf("pcall config file failed");
        goto ERR_HANDLE;
    }

    if (!getNumber(L, "WindowWidth", &config->WindowWidth)) {
        goto ERR_HANDLE;
    }
    if (!getNumber(L, "WindowHeight", &config->WindowHeight)) {
        goto ERR_HANDLE;
    }
    if (!getString(L, "Title", config->WindowTitle)) {
        goto ERR_HANDLE;
    }
    if (!getString(L, "EntryFile", config->LuaEntryFile)) {
        goto ERR_HANDLE;
    }

    lua_close(L);
    return 1;

ERR_HANDLE:
    lua_close(L);
    return 0;
}

lua_State* L = NULL;

void GameStart() {
    lua_getglobal(L, "GameStart");
    if (lua_pcall(L, 0, 0, 0)) {
        GAME_LOG_ERROR("call GameStart function in lua failed");
        const char* error = luaL_checkstring(L, -1);
        GAME_LOG_ERROR("[Lua Error]: %s", error);
    }
}

void GameLoop() {
    lua_getglobal(L, "GameLoop");
    if (lua_pcall(L, 0, 0, 0)) {
        GAME_LOG_ERROR("call GameLoop function in lua failed");
        const char* error = luaL_checkstring(L, -1);
        GAME_LOG_ERROR("[Lua Error]: %s", error);
    }
}

void GameQuit() {
    lua_getglobal(L, "GameQuit");
    if (lua_pcall(L, 0, 0, 0)) {
        GAME_LOG_ERROR("call GameQuit function in lua failed");
        const char* error = luaL_checkstring(L, -1);
        GAME_LOG_ERROR("[Lua Error]: %s", error);
    }
}

int main() {
    Config config;
    if (!ReadConfig(&config)) {
        return 1;
    }

    L = luaL_newstate();
    if (!L) {
        printf("lua init failed\n");
        return 1;
    }
    luaL_openlibs(L);

    if (luaL_loadfile(L, config.LuaEntryFile)) {
        createLuaEntryFile();
        ENGINE_LOG_CRITICAL("%s load and execute failed, create main.lua for you\n", config.LuaEntryFile);
    }

    if (lua_pcall(L, 0, 0, 0)) {
        ENGINE_LOG_ERROR("[Lua Error]: %s", luaL_checkstring(L, 1));
        return 1;
    }

    Hazel_Init(config.WindowTitle, config.WindowWidth, config.WindowHeight);
    Hazel_RegistGameFuncs(GameStart, GameLoop, GameQuit);

    Hazel_Run();

    Hazel_Quit();
    lua_close(L);
    return 0;
}
