#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>

PSP_MODULE_INFO("PlatformerPSP", 0, 1, 0);

#define SCREEN_W 480
#define SCREEN_H 272

int playerX = 50;
int playerY = 200;
int velY = 0;
int isJumping = 0;

int groundY = 200;

void drawRect(int x, int y, int w, int h, int color) {
    // simples placeholder (PSP real usa GU, aqui é lógico simplificado)
}

int main() {
    pspDebugScreenInit();

    SceCtrlData pad;

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        // esquerda / direita
        if (pad.Buttons & PSP_CTRL_LEFT) playerX -= 3;
        if (pad.Buttons & PSP_CTRL_RIGHT) playerX += 3;

        // pulo
        if ((pad.Buttons & PSP_CTRL_CROSS) && !isJumping) {
            velY = -10;
            isJumping = 1;
        }

        // física simples
        velY += 1;
        playerY += velY;

        if (playerY >= groundY) {
            playerY = groundY;
            velY = 0;
            isJumping = 0;
        }

        pspDebugScreenSetXY(0,0);
        pspDebugScreenPrintf("PlatformerPSP\nX:%d Y:%d", playerX, playerY);

        sceDisplayWaitVblankStart();
    }

    return 0;
}
