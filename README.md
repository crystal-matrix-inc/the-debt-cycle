# Welcome to The Debt Cycle!

'The Debt Cycle' is a digital card game about freeing yourself from the debt loop! Play with up to 3 AI opponents who will try to pull you down like crabs in a bucket. Made by [pranavcl](https://github.com/pranavcl), [jrrom](https://github.com/jrrom) and [VJ3](https://github.com/vj031206).

    Lead Programmer: pranavcl
    Programmer / Music: jrrom
    Design Head: VJ3

---

## Screenshots

![Screenshot one of the game](/screenshots/1.png)
![Screenshot two of the game](/screenshots/2.png)
![Screenshot three of the game](/screenshots/3.png)

---

## Controls (for digital version):

- LMB to use a card
- Scroll wheel to see the rest of your hand (if it gets too big)

## Formal Game Rules:

There are 2 piles of cards:

- The Deck (containing 120 cards)
- The Discard Pile (initially containing no cards)

The composition of The Deck is:

- Debt Cards: 45
- Steal x1: 8
- Steal x2: 5
- Reverse: 6
- Gamble 30%: 15
- Gamble 50%: 8
- Gamble 70%: 4
- Discard: 15
- Double Turns: 6
- Draw x2: 5
- Draw x4: 3

At the start of the game, every player gets 4 debt cards (Note: these cards are NOT to be taken from the Deck, which means you will need an additional 4 debt cards for every player (if you're playing IRL). So, if you're playing with 4 people, you need 120 + (4*4) = 136 cards)

Another note if you're playing IRL: For the Steal x1, Steal x2, Draw x2 and Draw x4 cards you will need some way to generate a random number between 1 and [no. of players playing] (You can use something like 4-sided die, 6-sided die, etc.). Also, for the gamble cards (Gamble 30%, Gamble 50% and Gamble 70%) you will need some way to generate a random number between 1 and 10 (You can use something like a 10-sided die).

## Game "Loop":

    Each round contains 2 phases: a DRAW phase, and a PLAY/ACTION phase.
    During the DRAW phase, every player must draw a card from the Deck, one by one.
    The PLAY/ACTION phase commences after every player has drawn a card from the Deck.
    In the PLAY/ACTION phase, players can use their action cards (everything that's not a Debt card). After using an action card, place it in the discard pile.
    The game ends in 3 scenarios: 

    A player loses all their debt cards (if this happens, the player who lost all their debt won)
    It's been more than 10-20 rounds (or some other mutually agreed-upon number).
    The deck is out of cards.

In the latter two cases, the player with the least amount of debt cards is the winner of the game.

### Cards:

- Debt: You have to lose these by using your PLAY/ACTION cards. The 3 victory conditions are listed above.
- Steal x1: Roll a die. If it comes on a number 'X', give one of your debt cards to the player who plays 'X' turns after you. For example, if the dice comes on '1', give a debt card to the player next to you. If it comes on '2', give a debt card to the player next to the player next to you and so on.
- Steal x2: Similar to Steal x1, but instead of giving 1 debt card, give 2.
- Reverse: Change the order of player turns. If the cycle was counter-clockwise, start playing clockwise and vice-versa.
- Gamble 30%: Roll a 10-sided die. If the number that comes up is lesser than or equal to 3, put a debt card from your hand into the discard pile.
- Gamble 50%: Roll a 10-sided die. If the number that comes up is lesser than or equal to 5, put a debt card from your hand into the discard pile.
- Gamble 70%: Roll a 10-sided die. If the number that comes up is lesser than or equal to 7, put a debt card from your hand into the discard pile.
- Discard: Put a debt card from your hand into the discard pile.
- Double Turns: Play 2 action cards in your turn (EXCLUDING the double turn card).
- Draw x2: Similar to Steal x1 and Steal x2, but instead of giving a random player one of your debt cards, force them to draw 2 cards from the Deck.
- Draw x4: Similar to Draw x2, but the random player is forced to draw 4 cards from the Deck.

## Stack Used: 

- Language: C
- Library: Raylib
- Collaboration Platform: replit
- Art: InkScape, Canva
- Sound effects: Bfxr
- Background music: Bosca Ceoil

---
