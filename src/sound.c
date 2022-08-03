#include "hazel/sound.h"

void Hazel_LoadSound(const char* filename, const char* name) {
    char buf[1024] = {0};
    snprintf(buf, sizeof(buf), "OPEN %s ALIAS %s", filename, name);
    if (mciSendString(TEXT(buf), NULL, 0, NULL)) {
        ENGINE_LOG_ERROR("load sound %s (alias %s) failed", filename, name);
    }
}

void Hazel_PlaySound(const char* name) {
    char buf[1024] = {0};
    snprintf(buf, sizeof(buf), "PLAY %s FROM 0", name);
    if (mciSendString(TEXT(buf), NULL, 0, NULL)) {
        ENGINE_LOG_ERROR("play sound %s failed", name);
    }
}