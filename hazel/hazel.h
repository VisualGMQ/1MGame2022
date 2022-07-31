#ifndef __HAZEL_HAZEL_H__
#define __HAZEL_HAZEL_H__

#include "pch.h"
#include "window.h"
#include "renderer.h"

typedef void(*Hazel_BeforeGameStartFunc)(void);
typedef void(*Hazel_GameLoopFunc)(void);
typedef void(*Hazel_BeforeGameQuitFunc)(void);
typedef void(*Hazel_KeyInputFunc)(GLFWwindow*, int, int, int, int);
typedef void(*Hazel_MousePosInputFunc)(GLFWwindow*, double, double);
typedef void(*Hazel_MouseButtonInputFunc)(GLFWwindow*, int, int , int);

DLLEXPORT void Hazel_Init(const char* title, int w, int h);
DLLEXPORT void Hazel_Quit();
DLLEXPORT int Hazel_ShouldClose();
DLLEXPORT void Hazel_Run();
DLLEXPORT void Hazel_RegistGameFuncs(Hazel_BeforeGameStartFunc, Hazel_GameLoopFunc, Hazel_BeforeGameQuitFunc);
DLLEXPORT void Hazel_RegistInputFuncs(Hazel_KeyInputFunc, Hazel_MousePosInputFunc, Hazel_MouseButtonInputFunc);
DLLEXPORT int Hazel_GetKeyState(int key);
DLLEXPORT int Hazel_GetMouseBtnState(int btn);
DLLEXPORT void Hazel_SetCursorPos(double x, double y);
DLLEXPORT void Hazel_GetMousePos(double* x, double* y);
DLLEXPORT double Hazel_GetTime();
DLLEXPORT void Hazel_HideCursor();
DLLEXPORT void Hazel_ShowCursor();

#endif
