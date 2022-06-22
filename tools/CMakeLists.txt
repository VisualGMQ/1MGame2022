add_executable(main main.cpp gui.cpp)
target_link_libraries(main PRIVATE tinyengine imgui)
target_precompile_headers(main REUSE_FROM tinyengine)
