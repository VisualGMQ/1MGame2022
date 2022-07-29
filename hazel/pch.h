#ifndef __HAZEL_PCH_H__
#define __HAZEL_PCH_H__

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#endif