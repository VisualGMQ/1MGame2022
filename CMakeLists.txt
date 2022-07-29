cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(HazelLuaEngine
    VERSION 0.1.0
    DESCRIPTION "a tiny engine made with C and Lua for 1MGames"
    LANGUAGES C
)

set(HAZEL_CORE_NAME hazel_core)
set(HAZEL_EXECUTOR_NAME HazelRunner)
set(HAZEL_LUA_BRIDGE_NAME hazel)

include(cmake/FindLua.cmake)
include(cmake/FindGLFW.cmake)
include(cmake/FindGLAD.cmake)

# hazel
aux_source_directory(src HAZEL_SRC)
add_library(${HAZEL_CORE_NAME} STATIC ${HAZEL_SRC})
target_link_libraries(${HAZEL_CORE_NAME} PUBLIC glad glfw)
target_include_directories(${HAZEL_CORE_NAME} PUBLIC .)
target_compile_features(${HAZEL_CORE_NAME} PUBLIC c_std_11)

if(WIN32)
    target_compile_definitions(${HAZEL_CORE_NAME} PUBLIC WIN32)
endif(WIN32)

# sandbox
add_subdirectory(binding/lua)