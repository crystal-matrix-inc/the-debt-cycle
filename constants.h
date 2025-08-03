#include "raylib.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TARGET_FPS 60
#define SCREEN_TITLE "GMTK Game Jam 2025"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720

#define DECK_SIZE 120
#define HAND_SIZE 30

#define WAIT_TIME 2

#define TEXT_COLOUR WHITE

extern Sound endfx;
extern Sound cardsfx;

typedef enum {
  MENU,
  GAME,
  RESULT
} GameState;

extern GameState gameState;
extern int numberOfPlayers;

#endif
