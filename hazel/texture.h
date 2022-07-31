#ifndef __HAZEL_TEXTURE_H__
#define __HAZEL_TEXTURE_H__

#include "pch.h"

struct Hazel_Texture;
typedef struct Hazel_Texture Hazel_Texture;

DLLEXPORT Hazel_Texture* Hazel_LoadTexture(const char* filename);
DLLEXPORT Hazel_Texture* Hazel_CreateTexture(const unsigned char* data, int w, int h);
DLLEXPORT void Hazel_DestroyTexture(Hazel_Texture*);
DLLEXPORT void Hazel_TextureGetSize(Hazel_Texture*, int* w, int* h);
DLLEXPORT void Hazel_TextureSetAlpha(Hazel_Texture*, float);
DLLEXPORT void Hazel_TextureSetColor(Hazel_Texture*, float r, float g, float b);
DLLEXPORT float Hazel_TextureGetAlpha(Hazel_Texture*);
DLLEXPORT void Hazel_TextureGetColor(Hazel_Texture*, float* r, float* g, float* b);
void Hazel_BindTexture(Hazel_Texture*);

#endif
