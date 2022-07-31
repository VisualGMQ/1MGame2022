add_library(${HAZEL_LUA_BRIDGE_NAME} SHARED bridge.c)
target_link_libraries(${HAZEL_LUA_BRIDGE_NAME} PUBLIC ${HAZEL_CORE_NAME} lua)
target_precompile_headers(${HAZEL_LUA_BRIDGE_NAME} REUSE_FROM ${HAZEL_CORE_NAME})

add_executable(${HAZEL_EXECUTOR_NAME} executor.c)
target_link_libraries(${HAZEL_EXECUTOR_NAME} PUBLIC lua ${HAZEL_CORE_NAME})
