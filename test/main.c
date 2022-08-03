#include "hazel/hazel.h"
#include "hazel/renderer.h"
#include "hazel/sound.h"

Hazel_Texture* texture = NULL;

void GameStart() {
    Hazel_LoadSound("test/resources/2.wav", "MUSIC1");
    Hazel_PlaySound("MUSIC1");
    texture = Hazel_LoadTexture("test/resources/player.png");
    Hazel_TextureSetColor(texture, 0, 1, 0);
    Hazel_TextureSetAlpha(texture, 0.3);
}

void GameLoop() {
    Hazel_RenderSetClearColor(0.1, 0.1, 0.1, 1);
    Hazel_RenderClear();

    Hazel_RenderSetDrawColor(0.5, 0.1, 0.1, 1);
    Hazel_RenderDrawLine(0, 0, 800, 600);

    Hazel_RenderSetDrawColor(0, 0.8, 0.1, 1);
    Hazel_RenderDrawRect(100, 100, 200, 100);

    int w, h;
    Hazel_TextureGetSize(texture, &w, &h);
    Hazel_RenderDrawTexture(texture, 10, 0, 10, 10, 0, 0, w * 3, h * 3, FLIP_HORIZONTAL);

    Hazel_RenderSetDrawColor(0, 0, 0.9, 1);
    Hazel_RenderFillRect(100, 300, 200, 100);
}

void GameQuit() {
    Hazel_DestroyTexture(texture);
}

int main() {
    Hazel_Init("test", 800, 600);
    Hazel_RegistGameFuncs(GameStart, GameLoop, GameQuit);
    Hazel_Run();
    Hazel_Quit();
    return 0;
}
