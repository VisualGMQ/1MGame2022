#include "hazel/renderer.h"

struct {
    GLuint shader, vbo, vao;
    Hazel_Texture* whiteTexture;
    float currentR, currentG, currentB, currentA;
} RenderContext;

void loadFile(const char* filename, char* buf) {
    FILE* file = fopen(filename, "r+");
    if (!file) {
        fclose(file);
        ENGINE_LOG_CRITICAL("shader file %s load failed", filename);
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    len = fread(buf, 1, len, file);
    buf[len] = '\0';
    fclose(file);
}

GLuint createShader() {
    int bufSize = sizeof(GLchar) * 1024;
    GLchar* buf = malloc(bufSize);
    memset(buf, 0, bufSize);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    loadFile("shader/shader.vert", buf);
    GL_CALL(glShaderSource(vertexShader, 1, &buf, NULL));
    GL_CALL(glCompileShader(vertexShader));

    int success;
    char infoLog[1024];
    GL_CALL(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        ENGINE_LOG_ERROR("vertex shader compile error:\n%s", infoLog);
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadFile("shader/shader.frag", buf);
    GL_CALL(glShaderSource(fragShader, 1, &buf, NULL));
    GL_CALL(glCompileShader(fragShader));
    GL_CALL(glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success));
    if (!success) {
        GL_CALL(glGetShaderInfoLog(fragShader, sizeof(infoLog), NULL, infoLog));
        ENGINE_LOG_ERROR("fragment shader compile error:\n%s", infoLog);
    }

    GLuint shader = glCreateProgram();
    GL_CALL(glAttachShader(shader, vertexShader));
    GL_CALL(glAttachShader(shader, fragShader));
    GL_CALL(glLinkProgram(shader));

    GL_CALL(glGetProgramiv(shader, GL_LINK_STATUS, &success));
    if(!success) {
        GL_CALL(glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog));
        ENGINE_LOG_ERROR("shader link error:\n%s", infoLog);
    }

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragShader));

    return shader;
}

void setProjectUniform(int w, int h) {
    float left = 0, right = w, top = 0, bottom = h, near = -1, far = 1;
    float projectMat[] = {
                       2 / (right - left),                               0,                           0, 0,
                                        0,              2 / (top - bottom),                           0, 0,
                                        0,                               0,            2 / (near - far), 0,
          (right + left) / (left - right), (top + bottom) / (bottom - top), (near + far) / (far - near), 1,
    };
    GLint loc = glGetUniformLocation(RenderContext.shader, "Project");
    if (loc < 0) {
        ENGINE_LOG_ERROR("shader uniform variable Project not found");
    }
    GL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, projectMat));
}

void initBuffer() {
    GL_CALL(glGenBuffers(1, &RenderContext.vbo));
    if (RenderContext.vbo == 0) {
        ENGINE_LOG_CRITICAL("vertex buffer create failed");
    }
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, RenderContext.vbo));
}

void initVertexArray() {
    GL_CALL(glGenVertexArrays(1, &RenderContext.vao));
    if (RenderContext.vao == 0) {
        ENGINE_LOG_CRITICAL("vertex array create failed");
    }
    GL_CALL(glBindVertexArray(RenderContext.vao));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    GL_CALL(glEnableVertexAttribArray(1));
}


void Hazel_RenderInit(int w, int h) {
    RenderContext.shader = createShader();

    GL_CALL(glUseProgram(RenderContext.shader));
    setProjectUniform(w, h);

    initBuffer();
    initVertexArray();

    unsigned char data[] = {255, 255, 255, 255};
    RenderContext.whiteTexture = Hazel_CreateTexture((void*)data, 1, 1);

    GL_CALL(glActiveTexture(GL_TEXTURE0));
    Hazel_RenderSetDrawColor(1, 1, 1, 1);

    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void Hazel_RenderQuit() {
    Hazel_DestroyTexture(RenderContext.whiteTexture);
    GL_CALL(glDeleteProgram(RenderContext.shader));
    GL_CALL(glDeleteBuffers(1, &RenderContext.vbo));
    GL_CALL(glDeleteVertexArrays(1, &RenderContext.vao));
}

void Hazel_RenderSetClearColor(float r, float g, float b, float a) {
    GL_CALL(glClearColor(r, g, b, a));
}

void Hazel_RenderSetDrawColor(float r, float g, float b, float a) {
    if (RenderContext.currentR == r &&
        RenderContext.currentG == g &&
        RenderContext.currentB == b &&
        RenderContext.currentA == a) {
        return;
    }
    GLint loc = glGetUniformLocation(RenderContext.shader, "Color");
    if (loc < 0) {
        ENGINE_LOG_ERROR("shader uniform variable Color not found");
        return;
    }
    GL_CALL(glUniform4f(loc, r, g, b, a));
    RenderContext.currentR = r;
    RenderContext.currentG = g;
    RenderContext.currentB = b;
    RenderContext.currentA = a;
}

void Hazel_RenderClear() {
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT));
}

void Hazel_RenderSetViewport(int x, int y, int w, int h) {
    GL_CALL(glViewport(x, y, w, h));
}

void Hazel_RenderDrawLine(int x1, int y1, int x2, int y2) {
    Hazel_BindTexture(RenderContext.whiteTexture);
    float vertices[] = {
        x1, y1, 0, 0,
        x2, y2, 0, 0,
    };
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CALL(glDrawArrays(GL_LINES, 0, 2));
}

void Hazel_RenderDrawRect(int x, int y, int w, int h) {
    Hazel_BindTexture(RenderContext.whiteTexture);
    float vertices[] = {
        x, y, 0, 0,
        x + w, y, 0, 0,
        x + w, y + h, 0, 0,
        x, y + h, 0, 0,
    };
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CALL(glDrawArrays(GL_LINE_LOOP, 0, 4));
}

void Hazel_RenderFillRect(int x, int y, int w, int h) {
    Hazel_BindTexture(RenderContext.whiteTexture);
    float vertices[] = {
        x, y, 0, 0,
        x + w, y, 0, 0,
        x + w, y + h, 0, 0,

        x, y, 0, 0,
        x + w, y + h, 0, 0,
        x, y + h, 0, 0,
    };
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void swap(float* v1, float* v2) {
    float tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

void Hazel_RenderDrawTexture(Hazel_Texture* texture,
                             int rx, int ry, int rw, int rh,
                             int dx, int dy, int dw, int dh,
                             Hazel_RenderFlip flip) {
    if (!texture) return;
    float r, g, b, a = Hazel_TextureGetAlpha(texture);
    Hazel_TextureGetColor(texture, &r, &g, &b);
    Hazel_RenderSetDrawColor(r, g, b, a);
    Hazel_BindTexture(texture);

    int w, h;
    Hazel_TextureGetSize(texture, &w, &h);
    float p1x = dx, p1y = dy,
          p2x = dx + dw, p2y = dy,
          p3x = dx + dw, p3y = dy + dh,
          p4x = dx, p4y = dy + dh;
    if (flip & FLIP_HORIZONTAL) {
        swap(&p1x, &p2x);
        swap(&p3x, &p4x);
    }
    if (flip & FLIP_VERTICAL) {
        swap(&p1y, &p3y);
        swap(&p2y, &p4y);
    }
    float vertices[] = {
        p1x, p1y, rx / (float)w, ry / (float)h,
        p2x, p2y, (rx + rw) / (float)w, ry / (float)h,
        p3x, p3y, (rx + rw) / (float)w, (ry + rh) / (float)h,

        p1x, p1y, rx / (float)w, ry / (float)h,
        p3x, p3y, (rx + rw) / (float)w, (ry + rh) / (float)h,
        p4x, p4y, rx / (float)w, (ry + rh) / (float)h,
    };
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
}
