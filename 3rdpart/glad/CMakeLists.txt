aux_source_directory(src GLAD_SRC)

add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC include)
