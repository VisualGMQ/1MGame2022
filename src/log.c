#include "hazel/log.h"

const char* GetFilename(const char* fullpath) {
    const char* ptr = fullpath + strlen(fullpath);
    while (*ptr != '/' && *ptr != '\\' && ptr != fullpath) ptr--;
    return ptr == fullpath ? ptr : ptr + 1;
}
