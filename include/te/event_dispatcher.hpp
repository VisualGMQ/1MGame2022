#pragma once

#include "pch.hpp"
#include "scence.hpp"
#include "renderer.hpp"

namespace te {

void EventKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void EventMouseMotionCallback(GLFWwindow* window, double xpos, double ypos);
void EventMouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
void EventWindowSizeCallback(GLFWwindow* window, int width, int height);

}
