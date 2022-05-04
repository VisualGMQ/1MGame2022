#include "te/key.hpp"

namespace te {

bool Key::IsPress(int key) {
    return glfwGetKey(Engine::GetWindow(), key) == GLFW_PRESS;
}

bool Key::IsRelease(int key) {
    return glfwGetKey(Engine::GetWindow(), key) == GLFW_RELEASE;
}

}
