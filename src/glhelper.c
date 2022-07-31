#include "hazel/glhelper.h"

void GLClearError() {
    int loop = 0;
    while (loop < TRY_GET_GL_ERROR_MAX_LOOP && glGetError() != GL_NO_ERROR) { loop++; }
}

#define CASE(e) case e: return #e;

const char* GLGetErrorString(GLenum err) {
    switch (err) {
        CASE(GL_NO_ERROR)
        CASE(GL_INVALID_ENUM)
        CASE(GL_INVALID_VALUE)
        CASE(GL_INVALID_OPERATION)
        CASE(GL_OUT_OF_MEMORY)
        CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
        default:
            return "Unknown Error";
    }
}
