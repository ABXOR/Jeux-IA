#ifndef IG_H
#define IG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "puissance4.h"
#include <stdio.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600
#define CELL_SIZE 100

// Enum pour les modes de l'IA
typedef enum {
    AI_RANDOM,
    AI_MINIMAX
} AI_Mode;

// Fonction pour dessiner le plateau de jeu
void drawBoard(SDL_Renderer *renderer, Item *game);

// Fonction pour afficher l'écran de démarrage
AI_Mode showStartScreen(SDL_Renderer *renderer, TTF_Font *font);

// Fonction pour initialiser SDL_ttf
void initSDL_ttf();

// Fonction pour rendre du texte
void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);

#endif // IG_H
