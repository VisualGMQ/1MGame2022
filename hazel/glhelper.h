#ifndef __HAZEL_GLHELPER_H__
#define __HAZEL_GLHELPER_H__

#include "glad/glad.h"

#define TRY_GET_GL_ERROR_MAX_LOOP 10000

const char* GLGetErrorString(GLenum);

void GLClearError();

#define GL_CALL(statement) do { \
    GLClearError(); \
    statement; \
    GLenum ___err_inner_use = glGetError(); \
    if (___err_inner_use != GL_NO_ERROR) { GL_LOG_ERROR("%s", GLGetErrorString(___err_inner_use)); } \
} while(0)

#endif
