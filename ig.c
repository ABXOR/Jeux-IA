#include "ig.h"


void drawBoard(SDL_Renderer *renderer, Item *game) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue background
    SDL_RenderClear(renderer);

    // Draw the grid
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White lines
    for (int i = 1; i < ROWS; ++i) {
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, WINDOW_WIDTH, i * CELL_SIZE);
    }
    for (int j = 1; j < COLS; ++j) {
        SDL_RenderDrawLine(renderer, j * CELL_SIZE, 0, j * CELL_SIZE, WINDOW_HEIGHT);
    }

    // Draw the tokens
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (game->board[i * COLS + j] != game->blank) {
                SDL_Rect cell = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                if (game->board[i * COLS + j] == 'X') {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red token
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow token
                }
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }

    SDL_RenderPresent(renderer);
}