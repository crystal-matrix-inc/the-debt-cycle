#include "constants.h"
#include "raylib.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "cards.h"

// Card functions and other details

CardAmounts cardAmounts = {
    [DEBT] = 45,

    [STEAL1]   = 8,
    [STEAL2]   = 5,
    [REVERSE]  = 6,
    [GAMBLE30] = 15,
    [GAMBLE50] = 8,
    [GAMBLE70] = 4,
    [DISCARD]  = 15,
    [DOUBLE]   = 6,
    [DRAW2]    = 5,
    [DRAW4]    = 3
};

// Assuming deck is 120 cards
void generateDeck(CardType arr[]) {
    srand(time(0));

    // Creating the deck
    int index = 0;
    for (CardType type = 0; type < TOTAL_NUMBER; type++) {
        for (int count = 0; count < cardAmounts[type]; count++) {
            arr[index++] = type;
        }
    }

    // Shuffling the deck
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        CardType temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

const char* cardTypeToString(CardType type) {
    switch (type) {
        case NOCARD:   return "NOCARD";
        case DEBT:     return "DEBT";
        case STEAL1:   return "STEAL1";
        case STEAL2:   return "STEAL2";
        case REVERSE:  return "REVERSE";
        case GAMBLE30: return "GAMBLE30";
        case GAMBLE50: return "GAMBLE50";
        case GAMBLE70: return "GAMBLE70";
        case DISCARD:  return "DISCARD";
        case DOUBLE:   return "DOUBLE";
        case DRAW2:    return "DRAW2";
        case DRAW4:    return "DRAW4";
        default:       return "UNKNOWN";
    }
}

// Size is 1-indexed
CardType takeCardFromDeck(CardType deck[], int* deckSize) {
    return deck[--(*deckSize)];
}

// Generate player hand of 7 debt
void generateHand(Player* p) {
    for (int i = 0; i < 4; i++) {
        p->hand[i] = (PlayerCard){
            .type = DEBT,
        };
    }
    p->size = 4;
}

void addToHand(Player* p, CardType deck[], int* deckSize) {
    if(p->size >= HAND_SIZE) return;
    
    p->hand[p->size] = (PlayerCard) {
        .type = takeCardFromDeck(deck, deckSize)
    };
    p->size++;
    PlaySound(cardsfx);
}

void coordinatesHand(Player* p, int x, int y) {
    for (int i = 0; i < p->size; i++) {
        p->hand[i].x = x;
        p->hand[i].y = y;
    }
}

Vector2 getXY(Player p, int index) {
    return (Vector2){p.hand[index].x, p.hand[index].y};
}


// =====================
// ======= DECK ========
// =====================


void deckInit(Deck* deck) {
  deck->texture = LoadTexture("assets/deck.png");
  generateDeck(deck->cards);

  deck->x = SCREEN_WIDTH/2 - deck->texture.width/2 - 100;
  deck->y = SCREEN_HEIGHT/2 - deck->texture.height/2;
  deck->top = DECK_SIZE-1;

  // printf("%s\n", cardTypeToString(takeCardFromDeck(deck->cards, &deck->top)));
}

void deckUpdate(Deck* deck) {
  if(deck->top < 0) deck->top = 0;

  deck->hover = false;

  if(CheckCollisionRecs((Rectangle){deck->x, deck->y, deck->texture.width, deck->texture.height}, (Rectangle){GetMouseX(), GetMouseY(), 1, 1})) {
    deck->hover = true;
  }
}

void deckDraw(Deck* deck, Font fnt) {
  DrawTexture(deck->texture, deck->x, deck->y, WHITE);
  DrawTextEx(fnt, TextFormat("Deck (%d)", deck->top+1), (Vector2){deck->x - 20, deck->y + deck->texture.height + 10}, 24, 2, TEXT_COLOUR);
}

// =====================
// === DISCARD PILE ====
// =====================

void discardPileInit(DiscardPile* discardPile) {
    discardPile->texture = LoadTexture("assets/discard.png");
    discardPile->x = SCREEN_WIDTH/2 - discardPile->texture.width/2 + 100;
    discardPile->y = SCREEN_HEIGHT/2 - discardPile->texture.height/2;
}

void discardPileUpdate(DiscardPile* discardPile) {
    
}

void discardPileDraw(DiscardPile* discardPile, Font fnt) {
    DrawTexture(discardPile->texture, discardPile->x, discardPile->y, WHITE);
    DrawTextEx(fnt, TextFormat("Discard Pile (%d)", discardPile->top), (Vector2){discardPile->x - 50, discardPile->y + discardPile->texture.height + 10}, 24, 2, TEXT_COLOUR);
}
