#ifndef __HAZEL_RENDERER_H__
#define __HAZEL_RENDERER_H__

#include "pch.h"
#include "texture.h"

typedef enum {
    FLIP_NONE = 0x00,
    FLIP_HORIZONTAL = 0x01,
    FLIP_VERTICAL = 0x02,
    FLIP_BOTH = FLIP_HORIZONTAL | FLIP_VERTICAL,
} Hazel_RenderFlip;

DLLEXPORT void Hazel_RenderInit(int w, int h);
DLLEXPORT void Hazel_RenderQuit();

DLLEXPORT void Hazel_RenderSetClearColor(float r, float g, float b, float a);
DLLEXPORT void Hazel_RenderClear();
DLLEXPORT void Hazel_RenderSetDrawColor(float r, float g, float b, float a);
DLLEXPORT void Hazel_RenderSetViewport(int x, int y, int w, int h);

DLLEXPORT void Hazel_RenderDrawLine(int x1, int y1, int x2, int y2);
DLLEXPORT void Hazel_RenderDrawRect(int x, int y, int w, int h);
DLLEXPORT void Hazel_RenderFillRect(int x, int y, int w, int h);

DLLEXPORT void Hazel_RenderDrawTexture(Hazel_Texture *texture,
                                       int rx, int ry, int rw, int rh,
                                       int dx, int dy, int dw, int dh,
                                       Hazel_RenderFlip);

#endif
