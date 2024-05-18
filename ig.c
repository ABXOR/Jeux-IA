#include "ig.h"

void initSDL_ttf() {
    if (TTF_Init() != 0) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        exit(1);
    }
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

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

AI_Mode showStartScreen(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Display the start screen text
    SDL_Color color = {255, 255, 255, 255}; // White color
    renderText(renderer, font, "Choose AI Mode", 250, 100, color);
    renderText(renderer, font, "1. Random", 250, 200, color);
    renderText(renderer, font, "2. Minimax", 250, 300, color);

    SDL_RenderPresent(renderer);

    // Wait for the user to choose an AI mode
    AI_Mode mode = AI_RANDOM;
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
                mode = AI_RANDOM;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= 250 && x <= 450) {
                    if (y >= 200 && y <= 250) {
                        mode = AI_RANDOM;
                        running = 0;
                    } else if (y >= 300 && y <= 350) {
                        mode = AI_MINIMAX;
                        running = 0;
                    }
                }
            }
        }
    }

    return mode;
}
