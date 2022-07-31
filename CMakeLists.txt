cmake_minimum_required(VERSION 3.15)

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
include(cmake/FindSTB.cmake)

# hazel-core
aux_source_directory(src HAZEL_SRC)
add_library(${HAZEL_CORE_NAME} SHARED ${HAZEL_SRC})
target_link_libraries(${HAZEL_CORE_NAME} PUBLIC glad glfw stb_image)
target_include_directories(${HAZEL_CORE_NAME} PUBLIC .)
target_compile_features(${HAZEL_CORE_NAME} PUBLIC c_std_11)
target_precompile_headers(${HAZEL_CORE_NAME} PUBLIC hazel/pch.h)

if(WIN32)
    target_compile_definitions(${HAZEL_CORE_NAME} PUBLIC WIN32)
elseif(APPLE)
    target_compile_definitions(${HAZEL_CORE_NAME} PUBLIC APPLE)
endif()

# lua
add_subdirectory(binding/lua)

# test
add_subdirectory(test)

# install
install(
    TARGETS ${HAZEL_LUA_BRIDGE_NAME} lua ${HAZEL_EXECUTOR_NAME} ${HAZEL_CORE_NAME}
    DESTINATION ${CMAKE_SOURCE_DIR}/install
)

install(
    DIRECTORY shader
    DESTINATION ${CMAKE_SOURCE_DIR}/install
)
