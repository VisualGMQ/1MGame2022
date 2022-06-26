#pragma once
#include "pch.hpp"

namespace te {

#define TRY_GET_GL_ERROR_MAX_LOOP 100

const char* GLGetErrorString(GLenum);

inline void GLClearError() {
    int loop = 0;
    while (loop < TRY_GET_GL_ERROR_MAX_LOOP && glGetError() != GL_NO_ERROR) { loop++; }
}

#define GL_CALL(statement) do { \
    GLClearError(); \
    statement; \
    GLenum ___err_inner_use = glGetError(); \
    if (___err_inner_use != GL_NO_ERROR) { GL_LOG_ERROR("%s", GLGetErrorString(___err_inner_use)); } \
} while(0)

}
