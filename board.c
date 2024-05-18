#include "board.h"
#include "puissance4.h"
#include <stdlib.h>
#include <stdio.h>

// Function to initialize the game
Item *initGame() {
    Item *game = (Item *)malloc(sizeof(Item));
    if (game == NULL) {
        perror("Failed to allocate memory for game");
        exit(EXIT_FAILURE);
    }

    game->size = ROWS * COLS;
    game->board = (char *)malloc(game->size * sizeof(char));
    if (game->board == NULL) {
        perror("Failed to allocate memory for board");
        free(game); // Free the allocated memory for game
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < game->size; i++) {
        game->board[i] = '.';
    }
    game->blank = '.';
    game->depth = 0;
    game->parent = NULL;
    game->prev = NULL;
    game->next = NULL;

    return game;
}

// Function to initialize the board state
void initBoard(Item *node) {
    for (int i = 0; i < node->size; i++) {
        node->board[i] = node->blank;
    }
    node->depth = 0;
}

// Function to get a child board state after a move
Item *getChildBoard(Item *node, int pos) {
    Item *child = (Item *)malloc(sizeof(Item));
    if (child == NULL) {
        perror("Failed to allocate memory for child board");
        exit(EXIT_FAILURE);
    }
    child->size = node->size;
    child->board = (char *)malloc(child->size * sizeof(char));
    if (child->board == NULL) {
        perror("Failed to allocate memory for board");
        free(child); // Free the allocated memory for child
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < node->size; i++) {
        child->board[i] = node->board[i];
    }
    child->blank = node->blank;
    child->depth = node->depth + 1;
    child->parent = node;
    child->prev = NULL;
    child->next = NULL;

    insertToken(child, pos);
    return child;
}

// Function to evaluate the current state of the board
double evaluateBoard(Item *node) {
    if (checkWin(node)) {
        return (node->depth % 2 == 0) ? -1000 : 1000;
    }
    return 0;
}

// Function to print the current state of the board
void printBoard(const Item *game) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", game->board[i * COLS + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// // Function to free the memory allocated for an Item
// void freeItem(Item *node) {
//     if (node) {
//         if (node->board) {
//             free(node->board);
//         }
//         free(node);
//     }
// }
