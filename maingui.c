#include "ig.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    initSDL_ttf();

    SDL_Window *window = SDL_CreateWindow("Puissance 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 24); // Ensure you have a valid TTF file at the specified path
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    AI_Mode aiMode = showStartScreen(renderer, font);
    Item *game = initGame();

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int col = x / CELL_SIZE;
                if (insertToken(game, col)) {
                    if (checkWin(game)) {
                        printf("Player %d wins!\n", (game->depth % 2 == 0) ? 2 : 1);
                        running = 0;
                    } else if (game->depth == ROWS * COLS) {
                        printf("It's a draw!\n");
                        running = 0;
                    } else {
                        int aiMove;
                        if (aiMode == AI_MINIMAX) {
                            aiMove = getBestMove(game);
                            printf("L'IA (Minimax) joue en colonne %d\n", aiMove);
                        } else {
                            aiMove = getRandomMove(game);
                            printf("L'IA (Aléatoire) joue en colonne %d\n", aiMove);
                        }
                        insertToken(game, aiMove);
                        if (checkWin(game)) {
                            printf("AI wins!\n");
                            running = 0;
                        }
                    }
                }
            }
        }

        drawBoard(renderer, game);
        SDL_Delay(100); // Delay to control frame rate
    }

    freeItem(game);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
