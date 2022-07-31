#include "hazel/window.h"

GLFWwindow* window = NULL;
int canvaW = 0, canvaH = 0;

void Hazel_GetCanvaSize(int* w, int* h) {
    if (w) *w = canvaW;
    if (h) *h = canvaH;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

int Hazel_InitWindow(const char* title, int width, int height) {
    if (!glfwInit()) {
        GAME_LOG_CRITICAL("glfw init failed");
        return 0;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 0;
    }

    canvaW = width;
    canvaH = height;

    GAME_LOG_INFO("window create OK");

    glfwMakeContextCurrent(window);

    gladLoadGL();
    GL_CALL(glViewport(0, 0, width, height));
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    return 1;
}

void Hazel_WindowGetSize(int* w, int* h) {
    glfwGetWindowSize(window, w, h);
}

void Hazel_WindowSetIcon(const char* filename) {
    GLFWimage icon;
    int channel;
    icon.pixels = stbi_load(filename, &icon.width, &icon.height, &channel, 0);
    if (!icon.pixels) {
        ENGINE_LOG_ERROR("icon %s load failed", filename);
    } else {
        glfwSetWindowIcon(window, 1, &icon);
        stbi_image_free(icon.pixels);
    }
}
