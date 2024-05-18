#ifndef IG_H
#define IG_H

#include <SDL2/SDL.h>
#include "board.h"
#include "puissance4.h"
#include <stdio.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600
#define CELL_SIZE 100

// Fonction pour dessiner le plateau de jeu
void drawBoard(SDL_Renderer *renderer, Item *game);

#endif // IG_H