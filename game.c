#include "constants.h"
#include "raylib.h"

#include "game.h"
#include "string.h"
#include "stdlib.h"
#include "result.h"

char CUSTOM_ALERT_TEXT[100];

// ======== Miscellaneous Functions =======

void drawPlayerNames(Font fnt, int playerNum) {
  DrawTextEx(fnt, "Player (use scroll wheel if there are too many cards)", (Vector2){SCREEN_WIDTH / 2 - 250, 600}, 24, 2,
             TEXT_COLOUR);

  DrawTextPro(fnt, "Computer 1", (Vector2){840, SCREEN_HEIGHT / 2 + 80},
              (Vector2){0, 0}, 270, 24, 2, TEXT_COLOUR);

            if(playerNum > 2) DrawTextEx(fnt, "Computer 2", (Vector2){SCREEN_WIDTH / 2 - 60, 100}, 24, 2,
             TEXT_COLOUR);

              if(playerNum > 3) DrawTextPro(fnt, "Computer 3", (Vector2){120, SCREEN_HEIGHT / 2 - 40}, (Vector2){0, 0}, 90, 24, 2, TEXT_COLOUR);
}

Alerts ALERT_ON = NO_ALERT;

void alert(Font fnt, const char text[]) {

  int fontSize = 48;
  float spacing = 2;
  Vector2 size = MeasureTextEx(fnt, text, fontSize, spacing);

  int rectangleSize = 200;

  // Background
  int x = (SCREEN_WIDTH - size.x - rectangleSize) / 2;
  int y = (SCREEN_HEIGHT - size.y - rectangleSize) / 2;
  DrawRectangle(x, y, size.x + rectangleSize, size.y + rectangleSize,
                Fade(BLACK, 0.7f));

  // Text
  DrawTextEx(
      fnt, text,
      (Vector2){(SCREEN_WIDTH - size.x) / 2, (SCREEN_HEIGHT - size.y) / 2},
      fontSize, spacing, WHITE);

  if (CheckCollisionRecs(
          (Rectangle){x, y, size.x + rectangleSize, size.y + rectangleSize},
          (Rectangle){GetMouseX(), GetMouseY(), 1, 1})) {
    // if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ALERT_ON = NO_ALERT;
  }
}

void alertSystemDraw(Font main_fnt, int round_num) {
  switch (ALERT_ON) {
  case DISPLAY_DRAW_PHASE:
    alert(main_fnt, TextFormat("ROUND %d - DRAW PHASE", round_num));
    break;
  case DISPLAY_PLAY_PHASE:
    alert(main_fnt, TextFormat("ROUND %d - PLAY PHASE", round_num));
    break;
  case DISPLAY_PLAYER_TURN:
    alert(main_fnt, TextFormat("YOUR TURN"));
    break;
  case DISPLAY_COMPUTER1_TURN:
    alert(main_fnt, TextFormat("COMPUTER 1 TURN"));
    break;
  case DISPLAY_COMPUTER2_TURN:
    alert(main_fnt, TextFormat("COMPUTER 2 TURN"));
    break;
  case DISPLAY_COMPUTER3_TURN:
    alert(main_fnt, TextFormat("COMPUTER 3 TURN"));
    break;
  case CUSTOM_ALERT:
    alert(main_fnt, CUSTOM_ALERT_TEXT);
    break;
  case CUSTOM_ALERT_WITH_TURN_CHANGE:
    alert(main_fnt, CUSTOM_ALERT_TEXT);
    break;
  }
}

bool alertWait(Game* game, Font fnt, Music music) {
    static double startTime = 0.0;
    static bool   waiting   = false;

    // If there's no alert, reset and let the game run
    if (ALERT_ON == NO_ALERT) {
        waiting = false;
        return false;
    }

    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      ALERT_ON = NO_ALERT;
    }  

    // Always update audio
    UpdateMusicStream(music);

    // If we just finished (or have never started) waiting for this alert, start now
    if (!waiting) {
        waiting   = true;
        startTime = GetTime();
        return true;    // block this frame too
    }

    // If we're still within WAIT_TIME, keep blocking
    if (GetTime() - startTime < WAIT_TIME) {
        return true;
    }

    // Time is up
    waiting = false;  // clear so next alert restarts

    if(ALERT_ON == CUSTOM_ALERT_WITH_TURN_CHANGE) {
      game->turnCount++;
      game->currentTurn = nextTurn(game);
      return true;
    }

    if(ALERT_ON == DISPLAY_DRAW_PHASE || ALERT_ON == DISPLAY_PLAY_PHASE) {
      switch (game->currentTurn) {
          case PLAYER:    
            ALERT_ON = DISPLAY_PLAYER_TURN;   break;
          case COMPUTER1: 
            ALERT_ON = DISPLAY_COMPUTER1_TURN; break;
          case COMPUTER2: 
            ALERT_ON = DISPLAY_COMPUTER2_TURN; break;
          case COMPUTER3: 
            ALERT_ON = DISPLAY_COMPUTER3_TURN; break;
      }
      return true;
    }

    if(ALERT_ON != CUSTOM_ALERT_WITH_TURN_CHANGE && ALERT_ON != DISPLAY_DRAW_PHASE && ALERT_ON != DISPLAY_PLAY_PHASE)
      ALERT_ON = NO_ALERT;

    /*

  switch (ALERT_ON) {
      case CUSTOM_ALERT_WITH_TURN_CHANGE:
        // Advance to next turn, *then* set the new alert
        game->turnCount++;
        game->currentTurn = nextTurn(game);
       break;

      case DISPLAY_DRAW_PHASE:
      case DISPLAY_PLAY_PHASE:
          switch (game->currentTurn) {
              case PLAYER:    
                ALERT_ON = DISPLAY_PLAYER_TURN;   break;
              case COMPUTER1: 
                ALERT_ON = DISPLAY_COMPUTER1_TURN; break;
              case COMPUTER2: 
                ALERT_ON = DISPLAY_COMPUTER2_TURN; break;
              case COMPUTER3: 
                ALERT_ON = DISPLAY_COMPUTER3_TURN; break;
          }
          break;

      default:  // covers CUSTOM_ALERT and all “TURN” alerts
          ALERT_ON = NO_ALERT;
          break;
  }
  
    */

    // printf("%d\n", waiting);

    // Return true one more frame so that if a new alert was just set,
    // it gets its own waiting cycle
    return true;
}

// void alertWait(Game* game, Font fnt, Music music) {
//   if(ALERT_ON == CUSTOM_ALERT_WITH_TURN_CHANGE) {
//     double startTime = GetTime();
//     while (GetTime() - startTime < WAIT_TIME) {
//     // Still pump the music buffer every frame:
//       UpdateMusicStream(music);
//     }
//     game->turnCount++;
//     game->currentTurn = nextTurn(game);
//     return;
//   }

//   if(ALERT_ON == DISPLAY_DRAW_PHASE || ALERT_ON == DISPLAY_PLAY_PHASE) {
//     double startTime = GetTime();
//     while (GetTime() - startTime < WAIT_TIME) {
//     // Still pump the music buffer every frame:
//       UpdateMusicStream(music);
//     }
//     switch (game->currentTurn) {
//       case PLAYER:
//         ALERT_ON = DISPLAY_PLAYER_TURN;
//         return;
//         break;
//       case COMPUTER1:
//         ALERT_ON = DISPLAY_COMPUTER1_TURN;
//         return;
//         break;
//       case COMPUTER2:
//         ALERT_ON = DISPLAY_COMPUTER2_TURN;
//         return;
//         break;
//       case COMPUTER3:
//         ALERT_ON = DISPLAY_COMPUTER3_TURN;
//         return;
//         break;
//       }
//   }
  
//   if(ALERT_ON == CUSTOM_ALERT) {
//     double startTime = GetTime();
//     while (GetTime() - startTime < WAIT_TIME / 2) {
//     // Still pump the music buffer every frame:
//       UpdateMusicStream(music);
//     }
//     ALERT_ON = NO_ALERT;
//   }
//   else if (ALERT_ON != NO_ALERT) {
//     double startTime = GetTime();
//     while (GetTime() - startTime < WAIT_TIME) {
//     // Still pump the music buffer every frame:
//       UpdateMusicStream(music);
//     }
//     ALERT_ON = NO_ALERT;
//   }
// }

// ===============================

// GAME FUNCTIONS

void gameInit(Game *game, int playerNum) {
  game->roundStarter = 0;
  deckInit(&game->gameDeck);
  discardPileInit(&game->discardPile);
  
  game->playerNum = playerNum;

  for (int i = 0; i < playerNum; i++) {
    generateHand(&game->players[i]);
  }

  game->currentTurn = PLAYER;
  game->phase = DRAW;
  game->cycle = ANTICLOCKWISE;

  game->roundNum = 1;
  game->turnCount = 1;

  ALERT_ON = DISPLAY_DRAW_PHASE;

  // For testing ONLY, remove in production

  // game->players[PLAYER].hand[0].type = STEAL2;
}

Turns nextTurn(Game *game) {
  Turns solution;

  int winner = -1;

  int debt_cards = 0;

  // If a player has no debt cards, they win
  
  for(int i = 0; i < game->playerNum; i++) {
    for(int j = 0; j < game->players[i].size; j++) {
      if(game->players[i].hand[j].type == DEBT)
        debt_cards++;
    }

    if(debt_cards == 0)
      winner = i;

    debt_cards = 0;
  }

  // If the deck is empty or more than 10 rounds have passed, the player with the least debt cards wins

  if(game->gameDeck.top <= 1 || game->roundNum > 10) {
    int min_idx = -1;
    int current = 0;
    
    for(int i = 0; i < game->playerNum; i++) {
      for(int j = 0; j < game->players[i].size; j++) {
        if(game->players[i].hand[j].type == DEBT)
          current++;
      }
      if(min_idx == -1 || current < min_idx)
        min_idx = i;
      current = 0;
    }

    winner = min_idx;
  }

  if(winner != -1) {
    gameState = RESULT;
    result_init(winner);
    return game->currentTurn;
  }

  for(int i = 0; i < game->playerNum; i++)
    if(game->players[i].doubleTurn) {
      game->players[i].doubleTurn = false;
      return game->currentTurn;
    }
  
  if (game->cycle == ANTICLOCKWISE)
    solution = (Turns)((game->currentTurn + 1) % game->playerNum);
  else
    solution =
        (Turns)((game->currentTurn - 1 + game->playerNum) % game->playerNum);

  if(game->turnCount <= game->playerNum) {
    switch (solution) {
    case PLAYER:
      ALERT_ON = DISPLAY_PLAYER_TURN;
      break;
    case COMPUTER1:
      ALERT_ON = DISPLAY_COMPUTER1_TURN;
      break;
    case COMPUTER2:
      ALERT_ON = DISPLAY_COMPUTER2_TURN;
      break;
    case COMPUTER3:
      ALERT_ON = DISPLAY_COMPUTER3_TURN;
      break;
    }
  } else {
    game->turnCount = 1;
    game->roundStarter = game->currentTurn;

    if(game->phase == DRAW) {
      game->phase = PLAY;
      ALERT_ON = DISPLAY_PLAY_PHASE;
    } else {
      game->phase = DRAW;
      ALERT_ON = DISPLAY_DRAW_PHASE;
      game->roundNum++;
    }
  }

  return solution;
}

void gameAction(Game *game) {
  /*if (game->turnCount > game->playerNum) {
    game->turnCount = 1;
    ALERT_ON = DISPLAY_PLAYER_TURN;
    return;
  }*/

  /*if (game->turnCount > game->playerNum) {
    game->turnCount = 1;
    game->roundStarter = game->currentTurn;
    
    if(game->phase == DRAW) {
      game->phase = PLAY;
      ALERT_ON = DISPLAY_PLAY_PHASE;
    } else {
      game->phase = DRAW;
      ALERT_ON = DISPLAY_DRAW_PHASE;
      game->roundNum++;
    }

    return;
  }*/

  // ========== DRAW PHASE ==========
  
  if (game->phase == DRAW) {

    if (game->currentTurn == PLAYER) {
      if (game->gameDeck.hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        addToHand(&game->players[PLAYER], game->gameDeck.cards,
                  &game->gameDeck.top);
        game->turnCount++;
        game->currentTurn = nextTurn(game);
      }
    } else {
      addToHand(&game->players[game->currentTurn], game->gameDeck.cards,
                &game->gameDeck.top);
      game->turnCount++;
      game->currentTurn = nextTurn(game);
    }

    // Temporary play phase code, add this in the play phase later
  }

  // ========== PLAY PHASE ==========

  if (game->phase == PLAY) {
    int selected_card = NOCARD;
    int selected_card_index = -1;
    
    if (game->currentTurn == PLAYER) {
      bool canAct = false;
      
      for(int i = 0; i < game->players[PLAYER].size; i++) {
        if(game->players[PLAYER].hand[i].type != DEBT && game->players[PLAYER].hand[i].type != NOCARD) {
          canAct = true;
          break;
        }
      }

      if(!canAct && ALERT_ON == NO_ALERT) {
        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
        strcpy(CUSTOM_ALERT_TEXT, "You cannot play any cards, skipping turn...");
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
          ALERT_ON = NO_ALERT;
        }
      }
      
      if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < game->players[PLAYER].size; i++) {
          if (game->players[PLAYER].hand[i].hover) {
            selected_card = game->players[PLAYER].hand[i].type;
            selected_card_index = i;
          }
        }
      }
    }

    if(game->currentTurn != PLAYER) {
      bool canAct = false;

      for(int i = 0; i < game->players[game->currentTurn].size; i++) {
        if(game->players[game->currentTurn].hand[i].type != DEBT && game->players[game->currentTurn].hand[i].type != NOCARD) {
          canAct = true;
          break;
        }
      }

      if(!canAct && ALERT_ON == NO_ALERT) {
        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
        strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d cannot play any cards, skipping turn...", game->currentTurn));
      } else {
        for(int i = 0; i < game->players[game->currentTurn].size; i++) {
          if(game->players[game->currentTurn].hand[i].type != DEBT && game->players[game->currentTurn].hand[i].type != NOCARD) {
            selected_card = game->players[game->currentTurn].hand[i].type;
            selected_card_index = i;
          }
        }
      }
    }

    int random_number = 0;

    if (selected_card != NOCARD) {
      // printf("%s\n", cardTypeToString(selected_card));
      switch (selected_card) {
      case DEBT: // ============ DEBT CARD LOGIC =============
        ALERT_ON = CUSTOM_ALERT;
        strcpy(CUSTOM_ALERT_TEXT, "One does not simply USE a debt card.");
        break;
      case STEAL1: // ============ STEAL1 CARD LOGIC =============
        while(random_number == game->currentTurn || random_number < 0 || random_number > game->playerNum-1)
          random_number = rand()%4;

        // Remove steal1 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = STEAL1;

        CardType c[] = {DEBT};
        int cSize = 1;

        addToHand(&game->players[random_number], c, &cSize);

        for(int i = 0; i < game->players[game->currentTurn].size; i++)
          if(game->players[game->currentTurn].hand[i].type == DEBT) {
            for(int j = i; j < game->players[game->currentTurn].size; j++)
              game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

            game->players[game->currentTurn].size--;
            break;
          }

        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

        if(game->currentTurn == PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("You stole from COMPUTER %d!", random_number));
        else if(random_number != PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d stole from COMPUTER %d!", game->currentTurn, random_number));
        else
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d stole from YOU!", game->currentTurn));

        break;
      case STEAL2: // ============ STEAL2 CARD LOGIC =============
        while(random_number == game->currentTurn || random_number < 0 || random_number > game->playerNum-1)
          random_number = rand()%4;

        // Remove steal2 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = STEAL2;

        CardType c1[] = {DEBT, DEBT};
        int c1Size = 2;

        addToHand(&game->players[random_number], c1, &c1Size);
        addToHand(&game->players[random_number], c1, &c1Size);

        for(int j = 0; j < 2; j++)
          for(int i = 0; i < game->players[game->currentTurn].size; i++)
            if(game->players[game->currentTurn].hand[i].type == DEBT) {
              for(int j = i; j < game->players[game->currentTurn].size; j++)
                game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

              game->players[game->currentTurn].size--;
              break;
            }

        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
        
        if(game->currentTurn == PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("You stole from COMPUTER %d!", random_number));
        else if(random_number != PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d stole from COMPUTER %d!", game->currentTurn, random_number));
        else
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d stole from YOU!", game->currentTurn));

        break;
      case REVERSE: // ============ REVERSE CARD LOGIC =============
        // Remove reverse card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = REVERSE;

        if(game->cycle == ANTICLOCKWISE)
          game->cycle = CLOCKWISE;
        else
          game->cycle = ANTICLOCKWISE;

        game->turnCount = 1;
        game->roundStarter = game->currentTurn;
        
        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

        if(game->currentTurn == PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, "You changed the playing order!");
        else
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d changed the playing order!", game->currentTurn));
        break;
      case GAMBLE30: // ============ GAMBLE30 CARD LOGIC =============
        // Remove gamble30 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = GAMBLE30;

        // Roll a d10
        random_number = 1 + rand()%9;

        // Winning logic

        if(random_number <= 3) {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You won the gamble! Discarding 1 debt...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d won the gamble! Discarding 1 debt...", game->currentTurn));

          for(int i = 0; i < game->players[game->currentTurn].size; i++)
            if(game->players[game->currentTurn].hand[i].type == DEBT) {
              game->discardPile.cards[game->discardPile.top++] = DEBT;

              for(int j = i; j < game->players[game->currentTurn].size; j++)
                game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

              game->players[game->currentTurn].size--;
              break;
            }
        } 

        // Losing logic

        else {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
          
          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You lost the gamble! Drawing 1 card...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d lost the gamble! Drawing 1 card...", game->currentTurn));
          
          addToHand(&game->players[game->currentTurn], game->gameDeck.cards, &game->gameDeck.top);
        }

        break;
      case GAMBLE50: // ============ GAMBLE50 CARD LOGIC =============
        // Remove gamble50 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = GAMBLE50;

        // Roll a d10
        random_number = 1 + rand()%9;

        // Winning logic

        if(random_number <= 5) {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You won the gamble! Discarding 1 debt...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d won the gamble! Discarding 1 debt...", game->currentTurn));

          for(int i = 0; i < game->players[game->currentTurn].size; i++)
            if(game->players[game->currentTurn].hand[i].type == DEBT) {
              game->discardPile.cards[game->discardPile.top++] = DEBT;

              for(int j = i; j < game->players[game->currentTurn].size; j++)
                game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

              game->players[game->currentTurn].size--;
              break;
            }
        } 

        // Losing logic

        else {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You lost the gamble! Drawing 1 card...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d lost the gamble! Drawing 1 card...", game->currentTurn));
          
          addToHand(&game->players[game->currentTurn], game->gameDeck.cards, &game->gameDeck.top);
        }
        break;
      case GAMBLE70: // ============ GAMBLE70 CARD LOGIC =============
        // Remove gamble70 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = GAMBLE70;

        // Roll a d10
        random_number = 1 + rand()%9;

        // Winning logic

        if(random_number <= 7) {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You won the gamble! Discarding 1 debt...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d won the gamble! Discarding 1 debt...", game->currentTurn));

          for(int i = 0; i < game->players[game->currentTurn].size; i++)
            if(game->players[game->currentTurn].hand[i].type == DEBT) {
              game->discardPile.cards[game->discardPile.top++] = DEBT;

              for(int j = i; j < game->players[game->currentTurn].size; j++)
                game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

              game->players[game->currentTurn].size--;
              break;
            }
        } 

        // Losing logic

        else {
          ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

          if(game->currentTurn == PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, "You lost the gamble! Drawing 1 card...");
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d lost the gamble! Drawing 1 card...", game->currentTurn));
          
          addToHand(&game->players[game->currentTurn], game->gameDeck.cards, &game->gameDeck.top);
        }
        break;
      case DISCARD: // ============ DISCARD CARD LOGIC =============
        // Remove discard card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = DISCARD;

        // Discard a DEBT card

        for(int i = 0; i < game->players[game->currentTurn].size; i++)
          if(game->players[game->currentTurn].hand[i].type == DEBT) {
            game->discardPile.cards[game->discardPile.top++] = DEBT;

            for(int j = i; j < game->players[game->currentTurn].size; j++)
              game->players[game->currentTurn].hand[j] = game->players[game->currentTurn].hand[j+1];

            game->players[game->currentTurn].size--;
            break;
          }

        // Alert and change turn
        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;

        if(game->currentTurn == PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, "You discarded a debt card.");
        else
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d discarded a debt card.", game->currentTurn));
        
        break;
      case DOUBLE: // ============ DOUBLE CARD LOGIC =============
        // Remove double card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = DOUBLE;

        game->players[game->currentTurn].doubleTurn = true;
        ALERT_ON = CUSTOM_ALERT;

        if(game->currentTurn == PLAYER)
          strcpy(CUSTOM_ALERT_TEXT, "You have a double turn!");
        else
          strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d has a double turn!", game->currentTurn));
        
        break;
      case DRAW2: // ============ DRAW2 CARD LOGIC =============
        // Remove draw2 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = DRAW2;

        // Pick a random player who will draw 2 cards

        while(random_number == game->currentTurn || random_number < 0 || random_number > game->playerNum-1)
          random_number = rand()%4;

        for(int k = 0; k < 2; k++)
          addToHand(&game->players[random_number], game->gameDeck.cards, &game->gameDeck.top);

         ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
        
         if(game->currentTurn == PLAYER)
           strcpy(CUSTOM_ALERT_TEXT, TextFormat("You made COMPUTER %d draw 2 cards!", random_number));
          else if(random_number != PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d made COMPUTER %d draw 2 cards!", game->currentTurn, random_number));
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d made YOU draw 2 cards!", game->currentTurn));
        break;
      case DRAW4: // ============ DRAW4 CARD LOGIC =============
        // Remove draw4 card from hand

        for(int i = selected_card_index; i < game->players[game->currentTurn].size; i++)
          game->players[game->currentTurn].hand[i] = game->players[game->currentTurn].hand[i+1];

        game->players[game->currentTurn].size--;
        game->discardPile.cards[game->discardPile.top++] = DRAW4;

        // Pick a random player who will draw 2 cards

        while(random_number == game->currentTurn || random_number < 0 || random_number > game->playerNum-1)
          random_number = rand()%4;

        for(int k = 0; k < 4; k++)
          addToHand(&game->players[random_number], game->gameDeck.cards, &game->gameDeck.top);

        ALERT_ON = CUSTOM_ALERT_WITH_TURN_CHANGE;
        
         if(game->currentTurn == PLAYER)
           strcpy(CUSTOM_ALERT_TEXT, TextFormat("You made COMPUTER %d draw 4 cards!", random_number));
          else if(random_number != PLAYER)
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d made COMPUTER %d draw 4 cards!", game->currentTurn, random_number));
          else
            strcpy(CUSTOM_ALERT_TEXT, TextFormat("COMPUTER %d made YOU draw 4 cards!", game->currentTurn));

        break;
      }
    }
  }
}

// ===============================
// Card Functions
// ===============================

// Collision
bool checkCollision(int offset_x, PlayerCard card, Texture2D texture,
                    int mouse_x, int mouse_y) {
  int card_x = card.x - offset_x;
  int card_y = card.y;

  return mouse_x >= card_x && mouse_x <= card_x + texture.width &&
         mouse_y >= card_y && mouse_y <= card_y + texture.height;
}

void updateCards(Game *game, Texture2D debtCard) {
  for (int i = 0; i < game->playerNum; i++) {
    for (int j = 0; j < game->players[i].size; j++) {
      // If player
      if (i == PLAYER) {
        game->players[i].hand[j].x = 200 + j * 60;
        game->players[i].hand[j].y = 640;

        if (game->players[i].offset_x > 1500)
          game->players[i].offset_x = 1500;
        if (game->players[i].offset_x < 0)
          game->players[i].offset_x = 0;
      }

      else if (i == COMPUTER1) {
        game->players[i].hand[j].x = 875;
        game->players[i].hand[j].y = 550 - j * 60;
      }

      else if (i == COMPUTER2) {
        game->players[i].hand[j].x = 700 - j * 60;
        game->players[i].hand[j].y = 80;
      }

      else if (i == COMPUTER3) {
        game->players[i].hand[j].x = 80;
        game->players[i].hand[j].y = 100 + j * 60;
      }
    }
  }

  Player *player = &game->players[PLAYER];

  // Checking collision for hover effect
  bool hover = false;
  for (int i = 0; i < player->size; i++) {
    if (checkCollision(player->offset_x, player->hand[i], debtCard, GetMouseX(),
                       GetMouseY())) {
      player->hand[i].y -= 20;
      player->hand[i].hover = true;
      hover = true;
    } else {
      player->hand[i].hover = false;
    }
  }

  if (hover || game->gameDeck.hover)
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  else
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

  // Checking scroll
  if (GetMouseWheelMove() < 0)
    player->offset_x -= 10;
  else if (GetMouseWheelMove() > 0)
    player->offset_x += 10;
}

void drawCards(
  Game *game, 
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
) {
  for (int i = 0; i < game->playerNum; i++) {
    Player current = game->players[i];

    if (i == PLAYER) {
      for (int i = 0; i < HAND_SIZE; i++) {
        if (i < current.size && current.hand[i].x - current.offset_x > 80) {
          switch (current.hand[i].type) {
            case NOCARD:
              break;
            case DEBT:
              DrawTexture(debtCard, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case STEAL1:
              DrawTexture(steal1Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case STEAL2:
              DrawTexture(steal2Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case REVERSE:
              DrawTexture(reverseCard, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case GAMBLE30:
              DrawTexture(gamble30Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case GAMBLE50:
              DrawTexture(gamble50Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case GAMBLE70:
              DrawTexture(gamble70Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case DISCARD:
              DrawTexture(discardCard, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case DOUBLE:
              DrawTexture(doubleCard, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case DRAW2:
              DrawTexture(draw2Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            case DRAW4:
              DrawTexture(draw4Card, current.hand[i].x - current.offset_x,
                current.hand[i].y, WHITE);
              break;
            default:
              DrawRectangle(current.hand[i].x - current.offset_x, current.hand[i].y,
                        45, 71, YELLOW);
              DrawText(cardTypeToString(current.hand[i].type),
                   current.hand[i].x - current.offset_x, current.hand[i].y, 8,
                   BLACK);
              break;
          }
        }
      }
    }

    else {
      int orientation = 0;
      Vector2 xyShift = {0};
      Vector2 widthHeight = {0};

      if (i == COMPUTER1) {
        orientation = 270;
        xyShift = (Vector2){0, -45};
        widthHeight = (Vector2){71, 45};
      }

      else if (i == COMPUTER2) {
        orientation = 180;
        xyShift = (Vector2){-45, -71};
        widthHeight = (Vector2){45, 71};
      }

      else if (i == COMPUTER3) {
        orientation = 90;
        xyShift = (Vector2){-71, 0};
        widthHeight = (Vector2){71, 45};
      }

      // Actually drawing

      for (int i = 0; i < HAND_SIZE; i++) {
        if (current.hand[i].type == DEBT) {
          DrawTextureEx(debtCard, getXY(current, i), orientation, 1, WHITE);
        } else if (current.hand[i].type != NOCARD) {
          DrawTextureEx(game->discardPile.texture, getXY(current, i), orientation, 1, WHITE);
        }
      }
    }
  }
}

