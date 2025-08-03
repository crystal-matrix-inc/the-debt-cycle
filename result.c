#include "raylib.h"
#include "result.h"
#include "constants.h"
#include "stdio.h"

int winner = 1;
int frameCount = 0;

void result_init(int w) {
  winner = w;
}

void result_update() {
  frameCount++;
  
  if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    gameState = MENU;
    printf("boo\n");
  }
}

void result_draw(Texture2D bg, Font fnt) {
  DrawTexture(bg, 0, 0, WHITE);

  if(winner == 0)
    DrawTextEx(fnt, "Player won!", (Vector2){SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50}, 48, 2, WHITE);
  else
    DrawTextEx(fnt, TextFormat("Computer %d won!", winner), (Vector2){SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 50}, 48, 2, WHITE);

  if(frameCount < 70) {
    DrawTextEx(fnt, TextFormat("Click to return to main menu", winner), (Vector2){SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT/2 + 150}, 48, 2, WHITE);
  }

  if(frameCount > 100) {
    frameCount = 0;
  }
}
