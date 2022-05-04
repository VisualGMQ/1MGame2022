#include "te/event_dispatcher.hpp"

namespace te {

void EventKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto scence = ScenceMgr::GetCurrentScence();
    if (scence) {
        if (action == GLFW_PRESS) {
            for (auto& layer : scence->GetLayers()) {
                if (!layer->OnEventKeyDown(key, scancode, mods)) {
                    break;
                }
            }
        } else if (action == GLFW_RELEASE) {
            for (auto& layer : scence->GetLayers()) {
                if (!layer->OnEventKeyUp(key, scancode, mods)) {
                    break;
                }
            }
        }
    }
}

void EventMouseMotionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto scence = ScenceMgr::GetCurrentScence();
    if (scence) {
        for (auto& layer : scence->GetLayers()) {
            if (!layer->OnEventMouseMotion(xpos, ypos)) {
                break;
            }
        }
    }
}

void EventMouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
    auto scence = ScenceMgr::GetCurrentScence();
    if (scence) {
        if (action == GLFW_PRESS) {
            for (auto& layer : scence->GetLayers()) {
                if (!layer->OnEventMouseButtonDown(button, mods)) {
                    break;
                }
            }
        } else if (action == GLFW_RELEASE) {
            for (auto& layer : scence->GetLayers()) {
                if (!layer->OnEventMouseButtonUp(button, mods)) {
                    break;
                }
            }
        }
    }
}

void EventWindowSizeCallback(GLFWwindow* window, int width, int height) {
    Renderer::SetViewport(0, 0, width, height);
    // NOTE currently don't have layer event
}

}
