#include "constants.h"
#include "raylib.h"

#include "cards.h"

#include "constants.h"
#include "raylib.h"

#ifndef GAME_H
#define GAME_H

// ========= GAME ==============

typedef enum { 
  PLAYER    = 0, 
  COMPUTER1 = 1, 
  COMPUTER2 = 2, 
  COMPUTER3 = 3
} Turns;

typedef enum { DRAW, PLAY } Phase;

typedef enum { ANTICLOCKWISE, CLOCKWISE } Cycle;

typedef struct {
  Deck gameDeck;
  DiscardPile discardPile;
  
  int playerNum;
  Player players[4];

  Turns currentTurn;
  int turnCount;
  Phase phase;
  Cycle cycle;

  int roundNum;
  int roundStarter;
} Game;

// Game functions
void gameInit(Game* game, int playerNum);
Turns nextTurn(Game* game);
void gameAction(Game* game);

// Card functions
bool checkCollision(int offset_x, PlayerCard card, Texture2D texture, int mouse_x, int mouse_y);
void updateCards(Game* game, Texture2D debtCard);
void drawCards(
  Game* game, 
  Texture2D debtCard,
  Texture2D steal1Card,
  Texture2D steal2Card,
  Texture2D reverseCard,
  Texture2D gamble30Card,
  Texture2D gamble50Card,
  Texture2D gamble70Card,
  Texture2D discardCard,
  Texture2D doubleCard,
  Texture2D draw2Card,
  Texture2D draw4Card
);

// ======== Miscellaneous Functions =======

void drawPlayerNames(Font fnt, int playerNum);

typedef enum {
  NO_ALERT,
  DISPLAY_PLAYER_TURN,
  DISPLAY_COMPUTER1_TURN,
  DISPLAY_COMPUTER2_TURN,
  DISPLAY_COMPUTER3_TURN,
  CHANGE_PHASE,
  CHANGE_TURN,
  DISPLAY_PLAY_PHASE,
  DISPLAY_DRAW_PHASE,
  PLAY_COMPUTER1,
  PLAY_COMPUTER2,
  PLAY_COMPUTER3,
  PLAY_PLAYER,
  CUSTOM_ALERT,
  CUSTOM_ALERT_WITH_TURN_CHANGE,
  SWITCH_TURN
} Alerts;

extern Alerts ALERT_ON;

void alert(Font fnt, const char text[]);
bool alertWait(Game* game, Font fnt, Music music);
void alertSystemDraw(Font main_fnt, int round_num);

// =============================

#endif
