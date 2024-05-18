#include "board.h"
#include "puissance4.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

// Function to insert a token into a column
int insertToken(Item *game, int col) {
    if (col < 0 || col >= COLS) {
        return 0; // Invalid column
    }
    for (int i = ROWS - 1; i >= 0; i--) {
        if (game->board[i * COLS + col] == game->blank) {
            game->board[i * COLS + col] = (game->depth % 2 == 0) ? 'X' : 'O';
            game->depth++;
            return 1; // Successful insertion
        }
    }
    return 0; // Column is full
}

// Function to check if the current player has won
int checkWin(const Item *game) {
    char playerToken = (game->depth % 2 == 0) ? 'O' : 'X';

    // Check horizontal
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 4; j++) {
            if (game->board[i * COLS + j] == playerToken &&
                game->board[i * COLS + j + 1] == playerToken &&
                game->board[i * COLS + j + 2] == playerToken &&
                game->board[i * COLS + j + 3] == playerToken) {
                return 1;
            }
        }
    }

    // Check vertical
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j < COLS; j++) {
            if (game->board[i * COLS + j] == playerToken &&
                game->board[(i + 1) * COLS + j] == playerToken &&
                game->board[(i + 2) * COLS + j] == playerToken &&
                game->board[(i + 3) * COLS + j] == playerToken) {
                return 1;
            }
        }
    }

    // Check diagonal (/)
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j <= COLS - 4; j++) {
            if (game->board[i * COLS + j] == playerToken &&
                game->board[(i + 1) * COLS + j + 1] == playerToken &&
                game->board[(i + 2) * COLS + j + 2] == playerToken &&
                game->board[(i + 3) * COLS + j + 3] == playerToken) {
                return 1;
            }
        }
    }

    // Check diagonal (\)
    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 3; j < COLS; j++) {
            if (game->board[i * COLS + j] == playerToken &&
                game->board[(i + 1) * COLS + j - 1] == playerToken &&
                game->board[(i + 2) * COLS + j - 2] == playerToken &&
                game->board[(i + 3) * COLS + j - 3] == playerToken) {
                return 1;
            }
        }
    }

    return 0;
}

// Function to evaluate the current state of the board for the AI
int evaluateBoardState(Item *game) {
    if (checkWin(game)) {
        return (game->depth % 2 == 0) ? -1000 : 1000;
    }
    return 0;
}

// Optimized Minimax algorithm with alpha-beta pruning
int minimax(Item *game, int depth, int alpha, int beta, int maximizingPlayer) {
    int score = evaluateBoardState(game);
    if (abs(score) == 1000 || depth == 0) {
        return score;
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (int col = 0; col < COLS; col++) {
            Item *child = getChildBoard(game, col);
            if (insertToken(child, col)) {
                int eval = minimax(child, depth - 1, alpha, beta, 0);
                maxEval = (eval > maxEval) ? eval : maxEval;
                alpha = (alpha > eval) ? alpha : eval;
                if (beta <= alpha) {
                    freeItem(child);
                    break;
                }
            }
            freeItem(child);
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int col = 0; col < COLS; col++) {
            Item *child = getChildBoard(game, col);
            if (insertToken(child, col)) {
                int eval = minimax(child, depth - 1, alpha, beta, 1);
                minEval = (eval < minEval) ? eval : minEval;
                beta = (beta < eval) ? beta : eval;
                if (beta <= alpha) {
                    freeItem(child);
                    break;
                }
            }
            freeItem(child);
        }
        return minEval;
    }
}

// Function to get the best move for the AI using Minimax
int getBestMove(Item *game) {
    int bestMove = -1;
    int bestValue = INT_MIN;

    for (int col = 0; col < COLS; col++) {
        Item *tempGame = getChildBoard(game, col);
        if (insertToken(tempGame, col)) {
            int moveValue = minimax(tempGame, 5, INT_MIN, INT_MAX, 0);
            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = col;
            }
        }
        freeItem(tempGame);
    }

    return bestMove;
}

// Function to get a random valid move for the AI
int getRandomMove(Item *game) {
    int validMove = 0;
    int col;

    while (!validMove) {
        col = rand() % COLS;
        validMove = insertToken(game, col);
    }

    return col;
}

void printInstructions() {
    printf("\nBienvenue dans le jeu Puissance 4 !\n");
    printf("Le plateau de jeu est une grille de 6 lignes et 7 colonnes.\n");
    printf("Vous jouez avec 'X' et l'IA joue avec 'O'.\n");
    printf("Pour jouer, entrez un numéro de colonne entre 0 et 6.\n");
    printf("Essayez d'aligner 4 jetons horizontalement, verticalement ou en diagonale pour gagner.\n");
    printf("Bonne chance !\n\n");
}

void printSeparator() {
    printf("+---+---+---+---+---+---+---+\n");
}

void printBoardNicely(const Item *game) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("| %c ", game->board[i * COLS + j]);
        }
        printf("|\n");
        printSeparator();
    }
}
