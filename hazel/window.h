#ifndef __HAZEL_WINDOW_H__
#define __HAZEL_WINDOW_H__

#include "pch.h"

extern GLFWwindow* window;

int Hazel_InitWindow(const char* title, int width, int height);
DLLEXPORT void Hazel_WindowGetSize(int* w, int* h);
DLLEXPORT void Hazel_GetCanvaSize(int* w, int* h);
DLLEXPORT void Hazel_WindowSetIcon(const char* filename);

#endif
