#include "gui.h"

// Dimensions de la fenêtre
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 600;

// Taille de la grille
const int CELL_SIZE = 100;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("Puissance 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    font = TTF_OpenFont("arial.ttf", 28);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }
}

void closeSDL() {
    TTF_CloseFont(font);
    font = NULL;

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    SDL_Quit();
}

void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void drawGrid(Item *game) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Dessiner les lignes de la grille
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i <= ROWS; i++) {
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, COLS * CELL_SIZE, i * CELL_SIZE);
    }
    for (int i = 0; i <= COLS; i++) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, ROWS * CELL_SIZE);
    }

    // Dessiner les jetons
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int x = j * CELL_SIZE + CELL_SIZE / 2;
            int y = i * CELL_SIZE + CELL_SIZE / 2;
            if (game->board[i * COLS + j] == 'X') {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
                SDL_RenderFillCircle(renderer, x, y, CELL_SIZE / 2 - 10);
            } else if (game->board[i * COLS + j] == 'O') {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Jaune
                SDL_RenderFillCircle(renderer, x, y, CELL_SIZE / 2 - 10);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void renderText(const char* message, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, color);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void showWinnerMessage(const char* message) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    renderText(message, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, textColor);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Recommencer", textColor);
    SDL_Rect restartButton = {SCREEN_WIDTH / 2 - textSurface->w / 2, SCREEN_HEIGHT / 2 + 20, textSurface->w + 20, textSurface->h + 10};
    SDL_FreeSurface(textSurface);

    // Draw button with border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
    SDL_RenderDrawRect(renderer, &restartButton);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green fill
    SDL_Rect innerButton = {restartButton.x + 2, restartButton.y + 2, restartButton.w - 4, restartButton.h - 4};
    SDL_RenderFillRect(renderer, &innerButton);
    
    renderText("Recommencer", restartButton.x + 10, restartButton.y + 5, textColor);

    SDL_RenderPresent(renderer);

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
                closeSDL();
                exit(0);
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= restartButton.x && x <= restartButton.x + restartButton.w &&
                    y >= restartButton.y && y <= restartButton.y + restartButton.h) {
                    quit = 1;
                    showMainMenu();
                }
            }
        }
    }
}

int handlePlayerMove(Item *game, int col) {
    if (insertToken(game, col)) {
        drawGrid(game);
        if (checkWin(game)) {
            char message[50];
            sprintf(message, "Le joueur %d gagne !", (game->depth % 2 == 0) ? 2 : 1);
            showWinnerMessage(message);
            return 1;
        } else if (game->depth == ROWS * COLS) {
            showWinnerMessage("Match nul !");
            return 1;
        }
    }
    return 0;
}

int aiMove(Item *game, int aiMode) {
    SDL_Delay(300); // Délai pour l'IA
    int col;
    if (aiMode == 1) {
        col = getBestMove(game);
    } else {
        col = getDfsMove(game);
    }
    insertToken(game, col);
    drawGrid(game);
    if (checkWin(game)) {
        showWinnerMessage("L'IA gagne !");
        return 1;
    }
    return 0;
}

void gameLoopPvP(Item *game) {
    int quit = 0;
    SDL_Event e;

    drawGrid(game);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int col = e.button.x / CELL_SIZE;
                if (handlePlayerMove(game, col)) {
                    quit = 1;
                }
            }
        }
    }
}

void gameLoopPvAI(Item *game, int aiMode) {
    int quit = 0;
    SDL_Event e;

    drawGrid(game);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int col = e.button.x / CELL_SIZE;
                if (game->depth % 2 == 0) {
                    if (handlePlayerMove(game, col)) {
                        quit = 1;
                    } else if (!checkWin(game) && game->depth % 2 == 1) {
                        if (aiMove(game, aiMode)) {
                            quit = 1;
                        }
                    }
                }
            }
        }
    }
}

void showAIMenu() {
    int quit = 0;
    SDL_Event e;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect aiRandom = {200, 150, 300, 100};
    SDL_Rect aiMinMax = {200, 300, 300, 100};

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
    SDL_RenderFillRect(renderer, &aiRandom);
    SDL_RenderFillRect(renderer, &aiMinMax);

    SDL_Color textColor = {0, 0, 0, 255}; // Noir
    renderText("IA DFS", aiRandom.x + 95, aiRandom.y + 35, textColor);
    renderText("IA MinMax", aiMinMax.x + 85, aiMinMax.y + 35, textColor);

    SDL_RenderPresent(renderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= 200 && x <= 500 && y >= 150 && y <= 250) {
                    Item *game = initGame();
                    gameLoopPvAI(game, 0); // Mode IA DFS
                    freeItem(game);
                    quit = 1;
                } else if (x >= 200 && x <= 500 && y >= 300 && y <= 400) {
                    Item *game = initGame();
                    gameLoopPvAI(game, 1); // Mode IA MinMax
                    freeItem(game);
                    quit = 1;
                }
            }
        }
    }
}

void showMainMenu() {
    int quit = 0;
    SDL_Event e;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect vsPlayer = {200, 150, 300, 100};
    SDL_Rect vsAI = {200, 300, 300, 100};

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu
    SDL_RenderFillRect(renderer, &vsPlayer);
    SDL_RenderFillRect(renderer, &vsAI);

    SDL_Color textColor = {255, 255, 255, 255}; // Blanc
    renderText("Joueur vs Joueur", vsPlayer.x + 40, vsPlayer.y + 35, textColor);
    renderText("Joueur vs IA", vsAI.x + 65, vsAI.y + 35, textColor);

    SDL_RenderPresent(renderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= 200 && x <= 500 && y >= 150 && y <= 250) {
                    Item *game = initGame();
                    gameLoopPvP(game); // Mode joueur vs joueur
                    freeItem(game);
                    quit = 1;
                } else if (x >= 200 && x <= 500 && y >= 300 && y <= 400) {
                    showAIMenu();
                    quit = 1;
                }
            }
        }
    }
}
