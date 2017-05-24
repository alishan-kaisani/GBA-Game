#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "text.h"
#include "StartScreen.h"
#include "GameOver.h"
#include "Cookie.h"
#include "Fire.h"

//Compiler Initialization
int cookieROW = 50;
int cookieCOL = 50;
fireST fires = {{0}, {0}, 0};
int playerSIZE = playerSIZEOG;
unsigned int colors[] = {RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, MAGENTA};
int colors_len = sizeof(colors)/sizeof(colors[0]);
unsigned int playerCOLOR = RED;

/*
    Run initial game calculations
    ALWAYS GENERATE COOKIE BEFORE player BECAUSE OF OVERLAPING CONDITIONALS
 */
playerST startGame() {
    //create dummy player temporarily to satisfy initial cookie placement
    playerST temp = {.row = 0, .col = 0, .numGrow = 0};
    generateCookie(temp);
    clearFires();
    generateFire(temp);
    drawString(150,60, "free", WHITE);
    drawImage3(0,0,STARTSCREEN_WIDTH,STARTSCREEN_HEIGHT,StartScreen) ;
    drawString(130,115, "PRESS START TO BEGIN", RED);
    playerSIZE = playerSIZEOG;
    playerCOLOR = colors[0];
    return generatePlayer();
}

void clearFires() {
    for (int i=0; i < fires.ndx; i++) {
        fires.row[i] = 0;
        fires.col[i] = 0;
    }
    fires.ndx = 0;
}

void endGAME(playerST GAMEplayer) {
    playerSIZE = playerSIZEOG;
    playerCOLOR = colors[0];
    clearFires();
    drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, GameOver);
    char buffer[100];
    sprintf(buffer, "FINAL SCORE IS: %i", GAMEplayer.numGrow);
    drawString(100, 60, buffer, MAGENTA);
    drawString(110,39, "PRESS SELECT TO PLAY AGAIN", CYAN);
}

int gameWonf(playerST player) {
    return player.numGrow >= 10;
}

/*
    Randomly initalizes player position outside of hitWall() areas
 */
playerST generatePlayer() {
    playerST outplayer;
    int playerROW = 1+rand()%(157-playerSIZE);
    int playerCOL = 1+rand()%(237-playerSIZE);
    int done = 0;
    while (!done) {
        done = 1;
        playerROW = rand()%(158-playerSIZE);
        playerCOL = rand()%(238-playerSIZE);
        if ((playerROW >= cookieROW-playerSIZE) && (playerROW <= cookieROW+playerSIZE)) {
            done = 0;
        }
        if ((playerCOL >= cookieCOL-playerSIZE) && (playerCOL <= cookieCOL+playerSIZE)) {
            done = 0;
        }
        for (int i = 0; i < fires.ndx; i++) {
            if ((playerROW >= fires.row[i]-playerSIZE) && (playerROW <= fires.row[i]+playerSIZE)) {
                done = 0;
            }
            if ((playerCOL >= fires.row[i]-playerSIZE) && (playerCOL <= fires.col[i]+playerSIZE)) {
                done = 0;
            }
        }
    }
    outplayer.row = playerROW;
    outplayer.col = playerCOL;
    outplayer.numGrow = 1;
    return outplayer;
}

/*
    Grows player size based on assumption of hitting cookie;
 */
void growplayer() {
    playerSIZE += 2;
}

/*
    Randomly generates new cookie positions & draws image
 */
void generateCookie(playerST GAMEplayer) {
    int cont = 1;
    while (cont) {
        cont = 0;
        cookieROW = (int) rand()%(159-COOKIE_HEIGHT);
        cookieCOL = (int) rand()%(239-COOKIE_WIDTH);
        if ((cookieROW >= GAMEplayer.row-COOKIE_HEIGHT) & (cookieROW <= GAMEplayer.row+COOKIE_HEIGHT)) {
            cont = 1;
        }
        if ((cookieCOL >= GAMEplayer.col-COOKIE_WIDTH) & (cookieCOL <= GAMEplayer.col+COOKIE_WIDTH)) {
            cont = 1;
        }
        for (int i = 0; i < fires.ndx; i++) {
            if ((cookieCOL >= fires.col[i]-COOKIE_WIDTH) & (cookieCOL <= fires.col[i]+COOKIE_WIDTH)) {
                cont = 1;
            }
            if ((cookieROW >= fires.row[i]-COOKIE_HEIGHT) & (cookieROW <= fires.row[i]+COOKIE_HEIGHT)) {
                cont = 1;
            }
        }
    }
}

void generateFire(playerST GAMEplayer) {
    if (fires.ndx == FIRES_CAP) {
        return;
    }
    int cont = 1;
    while (cont) {
        cont = 0;
        fires.row[fires.ndx] = (int) rand()%(159-FIRE_HEIGHT);
        fires.col[fires.ndx] = (int) rand()%(239-FIRE_WIDTH);
        if ((fires.row[fires.ndx] >= GAMEplayer.row-FIRE_HEIGHT) & (fires.row[fires.ndx] <= GAMEplayer.row+FIRE_HEIGHT)) {
            cont = 1;
        }
        if ((fires.col[fires.ndx] >= GAMEplayer.col-FIRE_WIDTH) & (fires.col[fires.ndx] <= GAMEplayer.col+FIRE_WIDTH)) {
            cont = 1;
        }
        if ((fires.col[fires.ndx] >= cookieCOL-FIRE_WIDTH) & (fires.col[fires.ndx] <= cookieCOL+FIRE_WIDTH)) {
            cont = 1;
        }
        if ((fires.row[fires.ndx] >= cookieROW-FIRE_HEIGHT) & (fires.row[fires.ndx] <= cookieROW+FIRE_HEIGHT)) {
            cont = 1;
        }
        for (int i = 0; i < fires.ndx; i++) {
            if ((fires.col[i] >= fires.col[fires.ndx]-FIRE_WIDTH) & (fires.col[i] <= fires.col[fires.ndx]+FIRE_WIDTH)) {
                cont = 1;
            }
            if ((fires.row[i] >= fires.row[fires.ndx]-FIRE_HEIGHT) & (fires.row[i] <= fires.row[fires.ndx]+FIRE_HEIGHT)) {
                cont = 1;
            }
        }
    }
    fires.ndx++;
}

/*
    Returns 1 if the player hit a wall
 */
int hitWall(playerST GAMEplayer) {
    int headRow = GAMEplayer.row;
    int headCol = GAMEplayer.col;
    if ((headRow) <= 0) {
        return 1;
    }
    if (headRow >= 159-playerSIZE) {
        return 1;
    }
    if ((headCol) <= 0) {
        return 1;
    }
    if (headCol >= 239-playerSIZE) {
        return 1;
    }
    return 0;
}

/*
    Returns 1 if the player hit a cookie
 */
int hitCookie(playerST GAMEplayer) {
    int headROW = GAMEplayer.row;
    int headCOL = GAMEplayer.col;
    if ((headROW >= cookieROW - playerSIZE) & (headROW <= cookieROW + COOKIE_HEIGHT)) {
        if ((headCOL >= cookieCOL - playerSIZE) & (headCOL <= cookieCOL + COOKIE_WIDTH)) {
            return 1;
        }
    }
    return 0;
}

/*
    Returns 1 if the player hits fire
 */
int hitFire(playerST GAMEplayer) {
    int headROW = GAMEplayer.row;
    int headCOL = GAMEplayer.col;
    for (int i = 0; i < fires.ndx; i++) {
        if ((headROW >= fires.row[i] - playerSIZE) & (headROW <= fires.row[i] + playerSIZE)) {
            if ((headCOL >= fires.col[i] - playerSIZE) & (headCOL <= fires.col[i] + playerSIZE)) {
                return 1;
            }
        }
    }
    return 0;
}
