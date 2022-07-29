set(LUA_SRC
    3rdlibs/lua/lapi.c
    3rdlibs/lua/lcode.c
    3rdlibs/lua/lctype.c
    3rdlibs/lua/ldebug.c
    3rdlibs/lua/ldo.c
    3rdlibs/lua/ldump.c
    3rdlibs/lua/lfunc.c
    3rdlibs/lua/lgc.c
    3rdlibs/lua/llex.c

    3rdlibs/lua/lmem.c
    3rdlibs/lua/lobject.c
    3rdlibs/lua/lopcodes.c
    3rdlibs/lua/lparser.c
    3rdlibs/lua/lstate.c
    3rdlibs/lua/lstring.c
    3rdlibs/lua/ltable.c

    3rdlibs/lua/ltm.c
    3rdlibs/lua/lundump.c
    3rdlibs/lua/lvm.c
    3rdlibs/lua/lzio.c
    3rdlibs/lua/ltests.c
    3rdlibs/lua/lauxlib.c
)
add_library(lua STATIC ${LUA_SRC})
target_include_directories(lua PUBLIC ./3rdlibs/lua)
target_compile_features(lua PRIVATE c_std_99)

# TODO if you use MinGW, maybe you need this
# target_compile_options(lua PRIVATE LUA_USE_LINUX)
if (NOT WIN32)
    target_compile_options(lua PRIVATE LUA_USE_LINUX)
endif()