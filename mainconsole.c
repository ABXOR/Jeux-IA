#include "puissance4.h"

int main() {
    srand(time(NULL)); // Initialize random number generator

    list_t openList_p;
    list_t closedList_p;

    initList(&openList_p);
    initList(&closedList_p);

    printInstructions();
    printSeparator();

    Item *initial_state = initGame();
    printBoardNicely(initial_state);

    int gameMode;
    printf("\nChoisissez le mode de jeu: 1 pour IA, 2 pour deux joueurs: ");
    while (scanf("%d", &gameMode) != 1 || (gameMode != 1 && gameMode != 2)) {
        printf("Choix invalide. Réessayez: ");
        while (getchar() != '\n'); // clear the buffer
    }

    if (gameMode == 1) {
        int aiMode;
        printf("\nChoisissez le mode IA: 1 pour Simple(DFS), 2 pour Extreme(minmax): ");
        while (scanf("%d", &aiMode) != 1 || (aiMode < 1 || aiMode > 3)) {
            printf("Choix invalide. Réessayez: ");
            while (getchar() != '\n'); // clear the buffer
        }

        while (1) {
            int col;

            if (initial_state->depth % 2 == 0) {
                printf("Votre tour (0-6) : ");
                while (scanf("%d", &col) != 1 || col < 0 || col >= COLS || !insertToken(initial_state, col)) {
                    printf("Mouvement invalide. Réessayez (0-6) : ");
                    while (getchar() != '\n'); // clear the buffer
                }
            } else {
                if (aiMode == 2) {
                    col = getBestMove(initial_state);
                    printf("L'IA (Minimax) joue en colonne %d\n", col);
                // } else if (aiMode == 3) {
                //     col = getAStarMove(initial_state);
                //     printf("L'IA (A*) joue en colonne %d\n", col);
                } else if (aiMode == 1) {
                    col = getDfsMove(initial_state);
                    printf("L'IA (Titi) joue en colonne %d\n", col);
                }
                insertToken(initial_state, col);
            }

            printBoardNicely(initial_state);

            if (checkWin(initial_state)) {
                printf("Le joueur %d gagne !\n", (initial_state->depth % 2 == 0) ? 2 : 1);
                break;
            }

            if (initial_state->depth == ROWS * COLS) {
                printf("Match nul !\n");
                break;
            }
        }
    } else if (gameMode == 2) {
        while (1) {
            int col;

            if (initial_state->depth % 2 == 0) {
                printf("Joueur 1 (0-6) : ");
            } else {
                printf("Joueur 2 (0-6) : ");
            }

            while (scanf("%d", &col) != 1 || col < 0 || col >= COLS || !insertToken(initial_state, col)) {
                printf("Mouvement invalide. Réessayez (0-6) : ");
                while (getchar() != '\n'); // clear the buffer
            }

            printBoardNicely(initial_state);

            if (checkWin(initial_state)) {
                printf("Le joueur %d gagne !\n", (initial_state->depth % 2 == 0) ? 2 : 1);
                break;
            }

            if (initial_state->depth == ROWS * COLS) {
                printf("Match nul !\n");
                break;
            }
        }
    }

    cleanupList(&openList_p);
    cleanupList(&closedList_p);

    return 0;
}
