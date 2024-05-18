#ifndef LIST_H
#define LIST_H

#include "item.h"

// Structure représentant une liste chaînée
typedef struct {
    int numElements;  // Nombre d'éléments dans la liste
    Item *first;      // Premier élément de la liste
    Item *last;       // Dernier élément de la liste
} list_t;

// Alloue de la mémoire pour un nouvel élément de la liste
Item *nodeAlloc();

// Libère la mémoire allouée pour un élément de la liste
void freeItem(Item *node);

// Initialise une liste chaînée
void initList(list_t *list_p);

// Retourne le nombre d'éléments dans la liste
int listCount(const list_t *list);

// Retourne un élément avec le même plateau ou NULL si non trouvé
Item *onList(const list_t *list, const char *board);

// Retourne et supprime le premier élément de la liste
Item *popFirst(list_t *list);

// Retourne et supprime le dernier élément de la liste
Item *popLast(list_t *list);

// Retourne et supprime le meilleur élément avec la valeur f minimale
Item *popBest(list_t *list);

// Ajoute un élément au début de la liste
void addFirst(list_t *list, Item *node);

// Ajoute un élément à la fin de la liste
void addLast(list_t *list, Item *node);

// Supprime un élément de la liste
void delList(list_t *list, Item *node);

// Vide la liste et libère les éléments
void cleanupList(list_t *list);

// Affiche le contenu de la liste
void printList(const list_t *list);

#endif // LIST_H
