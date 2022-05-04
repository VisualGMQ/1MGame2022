#pragma once
#include "pch.hpp"

namespace te {

const char* GLGetErrorString(GLenum);
inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR) {}
}

#define GL_CALL(statement) do { \
    GLClearError(); \
    statement; \
    GLenum ___err_inner_use = glGetError(); \
    if (___err_inner_use != GL_NO_ERROR) GL_LOG_ERROR("%s", GLGetErrorString(___err_inner_use)); \
} while(0)


}
