aux_source_directory(3rdlibs/glad/src GLAD_SRC)
add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC 3rdlibs/glad/include)
