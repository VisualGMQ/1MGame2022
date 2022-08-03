#ifndef __HAZEL_SOUND_H__
#define __HAZEL_SOUND_H__

#include <stdio.h>

#include "dllexport.h"
#include "windows.h"
#include "mmsystem.h"

DLLEXPORT void Hazel_LoadSound(const char* filename, const char* name);
DLLEXPORT void Hazel_PlaySound(const char* name);

#endif