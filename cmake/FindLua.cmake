set(LUA_SRC
    3rdlibs/lua/src/lapi.c
    3rdlibs/lua/src/lcode.c
    3rdlibs/lua/src/lctype.c
    3rdlibs/lua/src/ldebug.c
    3rdlibs/lua/src/ldo.c
    3rdlibs/lua/src/ldump.c
    3rdlibs/lua/src/lfunc.c
    3rdlibs/lua/src/lgc.c
    3rdlibs/lua/src/llex.c
    3rdlibs/lua/src/lmem.c
    3rdlibs/lua/src/lobject.c
    3rdlibs/lua/src/lopcodes.c
    3rdlibs/lua/src/lparser.c
    3rdlibs/lua/src/lstate.c
    3rdlibs/lua/src/lstring.c
    3rdlibs/lua/src/ltable.c
    3rdlibs/lua/src/ltm.c
    3rdlibs/lua/src/lundump.c
    3rdlibs/lua/src/lvm.c
    3rdlibs/lua/src/lzio.c
    3rdlibs/lua/src/lauxlib.c
    3rdlibs/lua/src/lbaselib.c
    3rdlibs/lua/src/lcorolib.c
    3rdlibs/lua/src/ldblib.c
    3rdlibs/lua/src/liolib.c
    3rdlibs/lua/src/lmathlib.c
    3rdlibs/lua/src/loadlib.c
    3rdlibs/lua/src/loslib.c
    3rdlibs/lua/src/lstrlib.c
    3rdlibs/lua/src/ltablib.c
    3rdlibs/lua/src/lutf8lib.c
    3rdlibs/lua/src/linit.c
)

add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ./3rdlibs/lua/src)
target_compile_features(lua PRIVATE c_std_99)

# TODO if you use MinGW, maybe you need this
# target_compile_options(lua PRIVATE LUA_USE_LINUX)
if (NOT WIN32)
    target_compile_options(lua PRIVATE LUA_USE_LINUX)
endif()