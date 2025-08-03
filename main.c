#include "raylib.h"

#include "result.h"
#include "constants.h"
#include "game.h"
#include "cards.h"

#include "menu.h"

// ======= INIT THINGS ========

GameState gameState = MENU;

Font main_fnt; // Main game font
Font alert_fnt;

// Textures
Texture2D debtCard;
Texture2D steal1Card;
Texture2D steal2Card;
Texture2D reverseCard;
Texture2D gamble30Card;
Texture2D gamble50Card;
Texture2D gamble70Card;
Texture2D discardCard;
Texture2D doubleCard;
Texture2D draw2Card;
Texture2D draw4Card;

Texture2D background;

Texture2D topDown;

// Game
Game game;
Music music;

Sound endfx;
Sound cardsfx;

int numberOfPlayers = 4;

// ======= INIT FUNCTION (Create game objects here) =======
void init() {
  InitAudioDevice();
  main_fnt = LoadFontEx("assets/libertinus_serif_reg.ttf", 24, 0, 0);
  alert_fnt = LoadFontEx("assets/libertinus_serif_reg.ttf", 48, 0, 0);

  debtCard = LoadTexture("assets/debt.png");
  steal1Card = LoadTexture("assets/steal1.png");
  steal2Card = LoadTexture("assets/steal2.png");
  reverseCard = LoadTexture("assets/reverse.png");
  gamble30Card = LoadTexture("assets/gamble3070.png");
  gamble50Card = LoadTexture("assets/gamble5050.png");
  gamble70Card = LoadTexture("assets/gamble7030.png");
  discardCard = LoadTexture("assets/discard1.png");
  doubleCard = LoadTexture("assets/doubleturn.png");
  draw2Card = LoadTexture("assets/draw2.png");
  draw4Card = LoadTexture("assets/draw4.png");

  background = LoadTexture("assets/background.png");

  music = LoadMusicStream("assets/gambling.ogg");

  endfx = LoadSound("assets/endfx.ogg");
  cardsfx = LoadSound("assets/cardsfx.ogg");
  
  SetMasterVolume(1.0f);
  SetMusicVolume(music, 1.0f);
  PlayMusicStream(music);

  gameInit(&game, 4);
}

// ======= UPDATE FUNCTION (Update game objects here) =======
void update() {
  UpdateMusicStream(music);
  
  if(gameState == GAME) {
    updateCards(&game, debtCard);

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      ALERT_ON = NO_ALERT;
    }
    
    if(alertWait(&game, main_fnt, music)) return;
    
    gameAction(&game);
  
    // Updating the deck
    deckUpdate(&game.gameDeck);
    discardPileUpdate(&game.discardPile);
  }
  else if(gameState == MENU) {
    menu_update(&game);
  }
  else if(gameState == RESULT) {
    result_update();
  }
}

// ======= DRAW FUNCTION (Draw game objects here) =======
void draw() {
  if(gameState == GAME) {
    drawPlayerNames(main_fnt, game.playerNum);
    deckDraw(&game.gameDeck, main_fnt);
    discardPileDraw(&game.discardPile, main_fnt);
  
    if(game.roundStarter == 0)
      DrawTextEx(main_fnt, "Rounds start/end from: Player", (Vector2){SCREEN_WIDTH/2-150, SCREEN_HEIGHT/2 +100}, 24, 2, TEXT_COLOUR);
    else
      DrawTextEx(main_fnt, TextFormat("Rounds start/end from: COMPUTER %d", game.roundStarter), (Vector2){SCREEN_WIDTH/2-200, SCREEN_HEIGHT/2 +100}, 24, 2, TEXT_COLOUR);
  
    if(game.cycle == ANTICLOCKWISE)
      DrawTextEx(main_fnt, "Playing order: Counter-clockwise", (Vector2){SCREEN_WIDTH/2-170, SCREEN_HEIGHT/2 +130}, 24, 2, TEXT_COLOUR);
    else
      DrawTextEx(main_fnt, "Playing order: Clockwise", (Vector2){SCREEN_WIDTH/2-125, SCREEN_HEIGHT/2 +130}, 24, 2, TEXT_COLOUR);
  
    // Draw player's cards
    drawCards(
      &game, 
      debtCard,
      steal1Card,
      steal2Card,
      reverseCard,
      gamble30Card,
      gamble50Card,
      gamble70Card,
      discardCard,
      doubleCard,
      draw2Card,
      draw4Card
    );

    // Alert system
    alertSystemDraw(alert_fnt, game.roundNum);
  }
  else if(gameState == MENU) {
    menu_draw(alert_fnt, main_fnt);
  }
  else if(gameState == RESULT) {
    result_draw(background, alert_fnt);
  }
}

// ======= MAIN FUNCTION (Do not modify) =======

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

  SetTargetFPS(TARGET_FPS);
  init();

  // Game loop
  while (!WindowShouldClose()) {
    update();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTextureEx(background, (Vector2){ 0, 0}, 0, 1, WHITE);

    draw();

    EndDrawing();

    // if(gameState == GAME) {
    //   // Put timer here
    //   alertWait(&game, alert_fnt, music);
    // }
  }

  CloseWindow();

  // Free resources
  UnloadFont(main_fnt);
  UnloadFont(alert_fnt);
  
  UnloadTexture(debtCard);
  UnloadTexture(steal1Card);
  UnloadTexture(steal2Card);
  UnloadTexture(reverseCard);
  UnloadTexture(gamble30Card);
  UnloadTexture(gamble50Card);
  UnloadTexture(gamble70Card);
  UnloadTexture(discardCard);
  UnloadTexture(doubleCard);
  UnloadTexture(draw2Card);
  UnloadTexture(draw4Card);

  UnloadTexture(background);

  UnloadTexture(game.gameDeck.texture);
  UnloadTexture(game.discardPile.texture);

  UnloadSound(endfx);
  UnloadSound(cardsfx);
  
  UnloadMusicStream(music);
  CloseAudioDevice();

  return 0;
}

