add_executable(main main.c)
target_link_libraries(main PUBLIC ${HAZEL_CORE_NAME})
target_precompile_headers(main REUSE_FROM ${HAZEL_CORE_NAME})