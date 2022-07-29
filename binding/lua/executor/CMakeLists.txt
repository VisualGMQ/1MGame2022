add_executable(${HAZEL_EXECUTOR_NAME} ./executor.c)
target_link_libraries(${HAZEL_EXECUTOR_NAME} PUBLIC ${HAZEL_CORE_NAME} lua)