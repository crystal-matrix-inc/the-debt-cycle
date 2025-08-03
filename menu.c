#include "raylib.h"
#include "constants.h"
#include "menu.h"
#include "game.h"

Rectangle fourPlayerButton = (Rectangle){SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100, 400, 50};
Rectangle threePlayerButton = (Rectangle){SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 400, 50};
Rectangle twoPlayerButton = (Rectangle){SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 + 100, 400, 50};

bool fourPlayerButtonHover = false;
bool threePlayerButtonHover = false;
bool twoPlayerButtonHover = false;

void menu_init() {
  
}

void menu_update(Game* game) {
  if(CheckCollisionRecs(fourPlayerButton, (Rectangle){GetMouseX(), GetMouseY(), 1, 1}))
    fourPlayerButtonHover = true;
  else
    fourPlayerButtonHover = false;

  if(CheckCollisionRecs(threePlayerButton, (Rectangle){GetMouseX(), GetMouseY(), 1, 1}))
    threePlayerButtonHover = true;
  else
    threePlayerButtonHover = false;

  if(CheckCollisionRecs(twoPlayerButton, (Rectangle){GetMouseX(), GetMouseY(), 1, 1}))
    twoPlayerButtonHover = true;
  else
    twoPlayerButtonHover = false;

  if(fourPlayerButtonHover || threePlayerButtonHover || twoPlayerButtonHover)
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  else
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

  if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    if(fourPlayerButtonHover) gameInit(game, 4);
    else if(threePlayerButtonHover) gameInit(game, 3);
    else if(twoPlayerButtonHover) gameInit(game, 2);
    
    if(fourPlayerButtonHover || threePlayerButtonHover || twoPlayerButtonHover) {
      gameState = GAME;
    }
  }
}

void menu_draw(Font fnt, Font fnt2) {
  DrawTextEx(fnt, "The Debt Cycle", (Vector2){SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 250}, 48, 2, WHITE);
  
  DrawTextEx(fnt2, "Made by pranavcl, jrrom & VJ3 for GMTK 2025\n\nUsing C, Raylib, InkScape, Canva, Bfxr & Bosca Ceoil", (Vector2){16, SCREEN_HEIGHT - 164}, 24, 2, WHITE);

  DrawRectangle(fourPlayerButton.x, fourPlayerButton.y, fourPlayerButton.width, fourPlayerButton.height, (fourPlayerButtonHover) ? LIGHTGRAY: WHITE);
  DrawTextEx(fnt2, "4-player mode (Recommended)", (Vector2){SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 90}, 24, 2, BLACK);
  
  DrawRectangle(threePlayerButton.x, threePlayerButton.y, threePlayerButton.width, threePlayerButton.height, (threePlayerButtonHover) ? LIGHTGRAY: WHITE);
  DrawTextEx(fnt2, "3-player mode", (Vector2){SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 + 10}, 24, 2, BLACK);
  
  DrawRectangle(twoPlayerButton.x, twoPlayerButton.y, twoPlayerButton.width, twoPlayerButton.height, (twoPlayerButtonHover) ? LIGHTGRAY: WHITE);
  DrawTextEx(fnt2, "2-player mode", (Vector2){SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 + 110}, 24, 2, BLACK);
  
}
