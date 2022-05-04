#include "te/mouse.hpp"

namespace te {

bool Mouse::IsLeftPress() {
    return glfwGetMouseButton(Engine::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool Mouse::IsLeftRelease() {
    return glfwGetMouseButton(Engine::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE;
}

bool Mouse::IsRightPress() {
    return glfwGetMouseButton(Engine::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}

bool Mouse::IsRightRelease() {
    return glfwGetMouseButton(Engine::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE;
}

Vec2 Mouse::Position() {
    int w, h;
    glfwGetWindowSize(Engine::GetWindow(), &w , &h);
    return RealPosition() * Size(w, h) / Engine::GetCanvaSize();
}

Vec2 Mouse::RealPosition() {
    double x, y;
    glfwGetCursorPos(Engine::GetWindow(), &x, &y);
    return Vec2(x, y);
}

}
