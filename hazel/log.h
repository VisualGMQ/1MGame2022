#include <stdio.h>
#include <string.h>
#include "dllexport.h"

DLLEXPORT const char* GetFilename(const char* fullpath);

#define ENGINE_LOG_CRITICAL(fmt, ...) do {printf("[Engine Critical][%d]%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout), exit(1); } while(0)
#define ENGINE_LOG_ERROR(fmt, ...)    do {printf("[Engine Error][%d][%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout); } while(0)
#define ENGINE_LOG_INFO(fmt, ...)     do {printf("[Engine Info][%d][%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout); } while(0)
#define ENGINE_LOG_WARN(fmt, ...)     do {printf("[Engine Warn][%d][%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout); } while(0)
#define ENGINE_LOG_DEBUG(fmt, ...)    do {printf("[Engine Debug][%d][%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout); } while(0)

#define GL_LOG_CRITICAL(fmt, ...) do {printf("[GL Critical][%d][%s - %s]: " fmt "\n", __LINE__, GetFilename(__FILE__), __FUNCTION__, ##__VA_ARGS__); fflush(stdout); exit(1); } while(0)
#define GL_LOG_ERROR(fmt, ...)    do {printf("[GL Error][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__);  fflush(stdout);} while(0)
#define GL_LOG_INFO(fmt, ...)     do {printf("[GL Info][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__);  fflush(stdout);} while(0)
#define GL_LOG_WARN(fmt, ...)     do {printf("[GL Warn][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__);  fflush(stdout);} while(0)
#define GL_LOG_DEBUG(fmt, ...)    do {printf("[GL Debug][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__); fflush(stdout);} while(0)

#define GAME_LOG_CRITICAL(fmt, ...) do {printf("[Game Critical][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__); fflush(stdout); } while(0)
#define GAME_LOG_ERROR(fmt, ...)   do {printf("[Game Error][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__); fflush(stdout);} while(0)
#define GAME_LOG_INFO(fmt, ...)     do {printf("[Game Info][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__);  fflush(stdout);} while(0)
#define GAME_LOG_WARN(fmt, ...)     do {printf("[Game Warn][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__);  fflush(stdout);} while(0)
#define GAME_LOG_DEBUG(fmt, ...)    do {printf("[Game Debug][%d][%s - %s]: " fmt "\n", __LINE__, __FUNCTION__, GetFilename(__FILE__), ##__VA_ARGS__); fflush(stdout);} while(0)
