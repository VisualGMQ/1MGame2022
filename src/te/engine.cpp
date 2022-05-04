#include "te/engine.hpp"

namespace te {

GLFWwindow* Engine::window_ = nullptr;
Timer Engine::steadyTimer_;
Size Engine::initWindowSize_;


void error_callback(int error, const char* description) {
    fprintf(stderr, "[GLFW Error]: %s\n", description);
}

void Engine::Init(const char* title, int w, int h) {
    initGLFW();
    window_ = createWindow(title, w, h);
    if (!window_) {
        glfwTerminate();
        ENGINE_LOG_CRITICAL("window create failed");
    }
    initGL();
    setEventDispatcher();
    glfwSwapInterval(1);
    GLClearError();

    initWindowSize_.Set(w, h);

    Renderer::Init(w, h);
}

void Engine::initGLFW() {
    if (!glfwInit()) {
        glfwTerminate();
        ENGINE_LOG_CRITICAL("glfw init failed");
    }
    glfwSetErrorCallback(error_callback);
}

GLFWwindow* Engine::createWindow(const char* title, int w, int h) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // only for MacOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    return glfwCreateWindow(w, h, title, NULL, NULL);
}

void Engine::initGL() {
    glfwMakeContextCurrent(window_);
    if (!gladLoadGL()) {
        glfwTerminate();
        ENGINE_LOG_CRITICAL("glad load gl failed");
    }
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);
}

void Engine::setEventDispatcher() {
    glfwSetKeyCallback(window_, EventKeyCallback);
    glfwSetMouseButtonCallback(window_, EventMouseBtnCallback);
    glfwSetCursorPosCallback(window_, EventMouseMotionCallback);
    glfwSetFramebufferSizeCallback(window_, EventWindowSizeCallback);
}

void Engine::Run() {
    while (!glfwWindowShouldClose(window_)) {
        Renderer::Clear();

        steadyTimer_.Update();

        auto scence = ScenceMgr::GetCurrentScence();
        if (scence) {
            scence->OnUpdate();
            scence->OnRender();
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void Engine::Quit() {
    Renderer::Quit();
    glfwDestroyWindow(window_);
    glfwTerminate();
}

}
