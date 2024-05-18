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

    int aiMode;
    printf("\n Choisissez le mode IA: 1 pour Minimax, 2 pour aléatoire: ");
    while (scanf("%d", &aiMode) != 1 || (aiMode != 1 && aiMode != 2)) {
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
            if (aiMode == 1) {
                col = getBestMove(initial_state);
                printf("L'IA (Minimax) joue en colonne %d\n", col);
                insertToken(initial_state, col);
            } else {
                col = getRandomMove(initial_state);
                printf("L'IA (Aléatoire) joue en colonne %d\n", col);
            }
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

    cleanupList(&openList_p);
    cleanupList(&closedList_p);

    return 0;
}
