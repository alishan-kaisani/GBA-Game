#include "myLib.h"
#include "Cookie.h"
#include "Fire.h"

unsigned short *videoBuffer =  (unsigned short*) 0x6000000;

//Screen Functions
void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row,col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color) {
	for(int r=0; r<height; r++) {
		REG_DMA3SAD = (unsigned int)&color;
		REG_DMA3DAD = (unsigned int)(&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void fillScreen3(volatile unsigned short color) {
    REG_DMA3SAD = (unsigned int) &color;
		REG_DMA3DAD = (unsigned int) (videoBuffer);
		REG_DMA3CNT = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}

void drawImage3(int row, int col, int width, int height, const unsigned short *image) {
	for (int r = 0; r < height; r++) {
		REG_DMA3SAD = (unsigned int) (&image[OFFSET(r, 0, width)]);
		REG_DMA3DAD = (unsigned int) (&videoBuffer[OFFSET(row+r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON;
	}
}

/*
    Draws player onto Screen
 */
void drawPlayer(playerST player) {
    drawRect(player.row, player.col,playerSIZE,playerSIZE, playerCOLOR);
}

/*
    Draws cookie onto Screen
 */
void drawCookie() {
    drawImage3(cookieROW,cookieCOL, COOKIE_WIDTH, COOKIE_HEIGHT, Cookie);
}

/*
    Draws fire onto Screen
 */
void drawFire() {
    for (int i=0; i < fires.ndx; i++) {
        drawImage3(fires.row[i],fires.col[i], FIRE_WIDTH, FIRE_HEIGHT, Fire);
    }
}
