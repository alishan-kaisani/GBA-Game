#include "myLib.h"
#include "text.h"
#include "GameWon.h"
#include <stdio.h>

enum GBAState {
	START,
	START_NODRAW,
	GAME,
    GAME_NODRAW,
    GAME_OVER,
    GAME_OVER_NODRAW,
    GAME_WON,
    GAME_WON_NODRAW
};

int main() {

  REG_DISPCNT = MODE_3 | BG2_EN;

  enum GBAState state = START;

  int START_PRESSED = 0; // 0 for false, 1 for true
  int SELECT_PRESSED = 0;
  int A_PRESSED = 0;
  int B_PRESSED = 0;

  int gameOVER = 0;
  int gameWon = 0;

  int rd = 1;
  int cd = 1;

  int move[4] = {0, 0, 0, 0};
  // [UP,DOWN,LEFT,RIGHT]

  char buffer[100];
  playerST GAMEplayer;
  GAMEplayer.numGrow = 0; //complete initialization of GAMEplayer;

  while(1) {
    switch(state) {
    case START:
      //Reset game settings
      gameOVER = 0;
      gameWon = 0;
      move[0] = 0;
      move[1] = 0;
      move[2] = 0;
      move[3] = 0;
      rd = 1;
      cd = 1;
      GAMEplayer = startGame();
      state = START_NODRAW;
      break;
    case START_NODRAW:
      //Pressing Start = start game;
      if (START_PRESSED == 0) {
        if (KEY_DOWN_NOW(BUTTON_START)) {
          START_PRESSED = 1;
          state = GAME;
        }
      }
      break;
    case GAME:
      ////////////////////////START GAME STATE///////////////////////////
      if(KEY_DOWN_NOW(BUTTON_UP)) {
        move[0] = 1;
        move[1] = 0;
        move[2] = 0;
        move[3] = 0;
      }
      if(KEY_DOWN_NOW(BUTTON_DOWN)) {
        move[0] = 0;
        move[1] = 1;
        move[2] = 0;
        move[3] = 0;
      }
      if (KEY_DOWN_NOW(BUTTON_LEFT)) {
        move[0] = 0;
        move[1] = 0;
        move[2] = 1;
        move[3] = 0;
      }
      if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
        move[0] = 0;
        move[1] = 0;
        move[2] = 0;
        move[3] = 1;
      }
      if (KEY_DOWN_NOW(BUTTON_A)) {
        A_PRESSED = 1;
        rd = (rd+1);
        cd = (cd+1);
      }
      if (KEY_DOWN_NOW(BUTTON_B)) {
        B_PRESSED = 1;
        if (rd > 1) {
            rd--;
        }
        if (cd > 1) {
            cd--;
        }
      }
      if (hitWall(GAMEplayer)) {
        gameOVER = 1;
      }
      if (hitFire(GAMEplayer)) {
        gameOVER = 1;
      }
      if (gameWonf(GAMEplayer)) {
        gameWon = 1;
      }
      if (hitCookie(GAMEplayer)) {
        growplayer();
        GAMEplayer.numGrow++;
        playerCOLOR = colors[(GAMEplayer.numGrow-1)%(colors_len)];
        generateCookie(GAMEplayer); //REspawn cookie;
        generateFire(GAMEplayer);
      }
      if (move[0]) {
        GAMEplayer.row -= rd;
      } else if (move[1]) {
        GAMEplayer.row += rd;
      } else if (move[2]) {
        GAMEplayer.col -=  cd;
      } else if (move[3]) {
        GAMEplayer.col += cd;
      }
      sprintf(buffer, "SCORE IS: %i", GAMEplayer.numGrow);
      waitForVblank();
      fillScreen3(BLACK);
      drawPlayer(GAMEplayer);
      drawCookie();
      drawFire(fires);
      drawString(150,85,buffer, RED);
      state = GAME_NODRAW;
      break;
      /////////////////////////END GAME STATE///////////////////////////
    case GAME_NODRAW:
      //Pressing Select = Go back to Start
      if (gameOVER) {
        state = GAME_OVER;
        break;
      }
      if (gameWon) {
        state = GAME_WON;
        break;
      }
      if (SELECT_PRESSED == 0) {
          if (KEY_DOWN_NOW(BUTTON_SELECT)) {
              SELECT_PRESSED = 1;
              state = START;
              break;
          }
      }
      state = GAME;
      break;
    case GAME_OVER:
      endGAME(GAMEplayer);
      gameWon = 0;
      state = GAME_OVER_NODRAW;
      break;
    case GAME_OVER_NODRAW:
      //Pressing Select = Go back to Start
      if (SELECT_PRESSED == 0) {
        if (KEY_DOWN_NOW(BUTTON_SELECT)) {
          SELECT_PRESSED = 1;
          state = START;
        }
      }
      break;
    case GAME_WON:
      gameWon = 0;
      char buffer[100];
      sprintf(buffer, "FINAL SCORE IS: %i", GAMEplayer.numGrow);
      waitForVblank();
      drawImage3(0, 0, GAMEWON_WIDTH, GAMEWON_HEIGHT, GameWon);
      drawString(100, 60, buffer, MAGENTA);
      drawString(110,39, "PRESS SELECT TO GO TO START", CYAN);
      state = GAME_WON_NODRAW;
    break;
    case GAME_WON_NODRAW:
      //Pressing Select = Go back to Start
      if (SELECT_PRESSED == 0) {
        if (KEY_DOWN_NOW(BUTTON_SELECT)) {
          SELECT_PRESSED = 1;
          state = GAME_NODRAW;
        }
      }
    }
    //OUTSIDE OF SWITCH STATES//
    if (!KEY_DOWN_NOW(BUTTON_START)) {
        START_PRESSED = 0;
    }
    if (!KEY_DOWN_NOW(BUTTON_SELECT)) {
        SELECT_PRESSED = 0;
    }
    if (!KEY_DOWN_NOW(BUTTON_A)) {
        A_PRESSED = 0;
    }
    if (!KEY_DOWN_NOW(BUTTON_B)) {
        B_PRESSED = 0;
    }
  }
  return 0;
}
