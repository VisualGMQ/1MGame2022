#include "hazel/hazel.h"

Hazel_GameLoopFunc gameLoopCallback = NULL;
Hazel_BeforeGameStartFunc beforeGameStartCallback = NULL;
Hazel_BeforeGameQuitFunc beforeGameQuitCallback = NULL;

void Hazel_Init(const char* title, int w, int h) {
    Hazel_InitWindow(title, w, h);
    Hazel_RenderInit(w, h);
}

void Hazel_Quit() {
    Hazel_RenderQuit();
    glfwTerminate();
}

int Hazel_ShouldClose() {
    return glfwWindowShouldClose(window);
}


void Hazel_RegistGameFuncs(Hazel_BeforeGameStartFunc before, Hazel_GameLoopFunc doing, Hazel_BeforeGameQuitFunc after) {
    beforeGameStartCallback = before;
    gameLoopCallback = doing;
    beforeGameQuitCallback = after;
}

void Hazel_Run() {
    if (beforeGameStartCallback) {
        beforeGameStartCallback();
    }

    while (!Hazel_ShouldClose()) {
        glfwPollEvents();

        if (gameLoopCallback) {
            gameLoopCallback();
        }

        glfwSwapBuffers(window);
    }

    if (beforeGameQuitCallback) {
        beforeGameQuitCallback();
    }
}

double Hazel_GetTime() {
    return glfwGetTime();
}

void Hazel_RegistInputFuncs(Hazel_KeyInputFunc keyInput,
                            Hazel_MousePosInputFunc mousePosInput,
                            Hazel_MouseButtonInputFunc mouseBtnInput) {
    glfwSetKeyCallback(window, keyInput);
    glfwSetCursorPosCallback(window, mousePosInput);
    glfwSetMouseButtonCallback(window, mouseBtnInput);
}

int Hazel_GetKeyState(int key) {
    return glfwGetKey(window, key);
}

void Hazel_GetMousePos(double* x, double* y) {
    glfwGetCursorPos(window, x, y);
}

void Hazel_SetCursorPos(double x, double y) {
    glfwSetCursorPos(window, x, y);
}

int Hazel_GetMouseBtnState(int btn) {
    return glfwGetMouseButton(window, btn);
}

void Hazel_HideCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Hazel_ShowCursor() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}