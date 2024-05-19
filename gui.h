#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "puissance4.h"
#include "list.h" // Ajout de cette ligne

// Dimensions de la fenêtre
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Taille de la grille
//extern const int ROWS;
//extern const int COLS;
extern const int CELL_SIZE;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;

// Initialisation et fermeture de SDL et SDL_ttf
void initSDL();
void closeSDL();

// Fonctions de rendu
void drawGrid(Item *game);
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius); // Ajout de cette ligne
void drawCircle(SDL_Renderer *renderer, int x, int y, int radius);
void renderText(const char* message, int x, int y, SDL_Color color);

// Boucle de jeu et gestion des mouvements
void handlePlayerMove(Item *game, int col);
void gameLoop(Item *game, int isAI, int aiMode);

// Affichage des menus
void showMainMenu();
void showAIMenu();

#endif // GUI_H