#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

#include "player_img.h"

PSP_MODULE_INFO("PlatformerPSP", 0, 1, 0);

#define SCREEN_W 480
#define SCREEN_H 272

int playerX = 50;
int playerY = 200;
int velY = 0;
int isJumping = 0;

void* fbp0;
void* fbp1;
void* zbp;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int image_size;
    unsigned int mipmap;
    unsigned int pixel_format;
} Image;

void initGraphics() {
    fbp0 = sceGeEdramGetAddr();
    fbp1 = fbp0 + (512 * 272 * 4);
    zbp  = fbp1 + (512 * 272 * 4);

    sceGuInit();
    sceGuStart(GU_DIRECT, NULL);

    sceGuDrawBuffer(GU_PSM_8888, fbp0, 512);
    sceGuDispBuffer(480, 272, fbp1, 512);
    sceGuDepthBuffer(zbp, 512);

    sceGuOffset(2048 - (480 / 2), 2048 - (272 / 2));
    sceGuViewport(2048, 2048, 480, 272);
    sceGuEnable(GU_TEXTURE_2D);

    sceGuFinish();
    sceGuSync(0,0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);
}

void drawSprite(Image* img, int x, int y) {
    sceGuTexMode(GU_PSM_8888, 0, 0, 0);
    sceGuTexImage(0, img->width, img->height, img->width, img+1);

    ScePspFVector3 pos = {x, y, 0};

    sceGumLoadIdentity();
    sceGumTranslate(&pos);

    sceGuDrawArray(GU_SPRITES,
        GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
        2, 0, img);
}

int main() {
    SceCtrlData pad;

    initGraphics();

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_LEFT) playerX -= 3;
        if (pad.Buttons & PSP_CTRL_RIGHT) playerX += 3;

        if ((pad.Buttons & PSP_CTRL_CROSS) && !isJumping) {
            velY = -10;
            isJumping = 1;
        }

        velY += 1;
        playerY += velY;

        if (playerY >= 200) {
            playerY = 200;
            velY = 0;
            isJumping = 0;
        }

        sceGuStart(GU_DIRECT, NULL);

        drawSprite((Image*)player_img, playerX, playerY);

        sceGuFinish();
        sceGuSync(0,0);

        sceDisplayWaitVblankStart();
    }

    return 0;
}
