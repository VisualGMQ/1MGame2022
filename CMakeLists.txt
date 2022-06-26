cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(DarkCube
        LANGUAGES CXX C
        VERSION 0.1.0
        DESCRIPTION "a game made for 1M Game Competition 2th(2022.8.6)")

####################
# 3rdlibs
####################
add_subdirectory(3rdpart/miniaudio)
add_subdirectory(3rdpart/glad)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_USE_WAYLAND ON)
add_subdirectory(3rdpart/glfw)
add_subdirectory(3rdpart/stb)

####################
# tinyengine build
####################
aux_source_directory(src/te tinyengine_src)
add_library(tinyengine STATIC ${tinyengine_src})
target_link_libraries(tinyengine PUBLIC miniaudio glad glfw stb_image)
target_include_directories(tinyengine PUBLIC include)
target_compile_features(tinyengine PUBLIC cxx_std_17)
target_precompile_headers(tinyengine PUBLIC include/te/pch.hpp)

if ("${CMAKE_BUILD_TYPE}" MATCHES "Release")
    target_compile_options(tinyengine PUBLIC -Os -fno-rtti -fno-exceptions)
endif()

####################
# game build
####################
aux_source_directory(src/game game_src)
aux_source_directory(src/gui gui_src)
add_executable(game ${game_src} ${gui_src})
target_link_libraries(game tinyengine)
target_precompile_headers(game REUSE_FROM tinyengine)

####################
# tools build
####################
add_subdirectory(3rdpart/imgui)
add_subdirectory(tools)

####################
# run game
####################

message(STATUS ${CMAKE_BINARY_DIR})

add_custom_target(
    run
    COMMAND ./${CMAKE_BINARY_DIR}/Debug/game.exe
)
