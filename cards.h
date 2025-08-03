#include "constants.h"
#include "raylib.h"

#ifndef CARDS_H
#define CARDS_H

// typedefs

typedef enum CardType {
  DEBT,

  STEAL1,
  STEAL2,
  REVERSE,
  GAMBLE30,
  GAMBLE50,
  GAMBLE70,
  DISCARD,
  DOUBLE,
  DRAW2,
  DRAW4,

  TOTAL_NUMBER,
  // TOTAL_NUMBER needs to be after all correct cards, please dont put anything
  // before TOTAL_NUMBER is used for generating the DECK!!
  NOCARD,
} CardType;

// PlayerCard struct
typedef struct PlayerCard {
  CardType type;
  int x;
  int y;
  bool hover;
} PlayerCard;

// Player struct

typedef struct Player {
  PlayerCard hand[HAND_SIZE];
  int size;
  int offset_x;
  bool doubleTurn;
} Player;

typedef int CardAmounts[TOTAL_NUMBER];

// cardAmounts array of cards and their amounts

extern CardAmounts cardAmounts;

// functions

void generateDeck(CardType arr[]);
const char *cardTypeToString(CardType type);
CardType takeCardFromDeck(CardType arr[], int *size);
void generateHand(Player *p);
void coordinatesHand(Player *p, int x, int y);
void addToHand(Player *p, CardType deck[], int *deckSize);
Vector2 getXY(Player p, int index);

// ======== DECK ========
typedef struct {
  Texture2D texture;
  CardType cards[DECK_SIZE];
  int x, y, top;
  bool hover;
} Deck;

void deckInit(Deck *deck);
void deckUpdate(Deck *deck);
void deckDraw(Deck *deck, Font fnt);

// ======== DISCARD PILE ========

typedef struct {
  Texture2D texture;
  CardType cards[DECK_SIZE];
  int x, y, top;
} DiscardPile;

void discardPileInit(DiscardPile *deck);
void discardPileUpdate(DiscardPile *deck);
void discardPileDraw(DiscardPile *deck, Font fnt);

#endif

