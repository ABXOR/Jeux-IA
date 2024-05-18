#ifndef PUISSANCE4_H
#define PUISSANCE4_H

#include "item.h"
#include "board.h"


// Function to insert a token into a column
int insertToken(Item *game, int col);

// Function to check if the current player has won
int checkWin(const Item *game);

// Function to evaluate the current state of the board for the AI
int evaluateBoardState(Item *game);

// Random algorithm
int getRandomMove(Item *game);

// Minimax algorithm with alpha-beta pruning
int minimax(Item *game, int depth, int alpha, int beta, int maximizingPlayer);

// Function to get the best move for the AI
int getBestMove(Item *game);

#endif // PUISSANCE4_H