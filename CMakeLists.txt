cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(1MGames2th
        LANGUAGES CXX C)

####################
# 3rdlibs
####################
add_subdirectory(3rdpart/miniaudio)
add_subdirectory(3rdpart/glad)
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

####################
# game build
####################
aux_source_directory(src/game game_src)
add_executable(game ${game_src})
target_link_libraries(game tinyengine)
target_precompile_headers(game REUSE_FROM tinyengine)
