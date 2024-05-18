#ifndef BOARD_H
#define BOARD_H

#include "item.h"
#include "list.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define ROWS 6
#define COLS 7

// Function to initialize the game
Item *initGame();

// Function to initialize the board state
void initBoard(Item *node);

// Function to get a child board state after a move
Item *getChildBoard(Item *node, int pos);

// Function to evaluate the current state of the board
double evaluateBoard(Item *node);

// Function to print the current state of the board
void printBoard(const Item *game);

// Function to free the memory allocated for an Item
void freeItem(Item *node);

#endif // BOARD_H
