#include "list.h"

/* Fonction pour allouer un nouvel élément de liste */
Item *nodeAlloc() {
    Item *node = (Item *)malloc(sizeof(Item));
    assert(node);

    node->board = NULL;
    node->parent = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->f = node->g = node->h = 0.0;

    return node;
}

/* Fonction pour libérer un élément de liste */
void freeItem(Item *node) {
    if (node) {
        if (node->board) {
            free(node->board);
        }
        free(node);
    }
}

/* Fonction pour initialiser une liste chaînée */
void initList(list_t *list_p) {
    assert(list_p);

    list_p->numElements = 0;
    list_p->first = NULL;
    list_p->last = NULL;
}

/* Fonction pour retourner le nombre d'éléments dans la liste */
int listCount(const list_t *list) {
    return list->numElements;
}

/* Fonction pour rechercher un élément dans la liste par le plateau */
Item *onList(const list_t *list, const char *board) {
    Item *test = list->first;
    while (test != NULL) {
        if (strcmp(test->board, board) == 0) {
            return test;
        }
        test = test->next;
    }
    return NULL;
}

/* Fonction pour retourner et supprimer le premier élément de la liste */
Item *popFirst(list_t *list) {
    if (list->numElements == 0) {
        return NULL;
    }

    Item *first = list->first;
    list->first = first->next;
    if (list->first) {
        list->first->prev = NULL;
    } else {
        list->last = NULL;
    }

    list->numElements--;
    return first;
}

/* Fonction pour retourner et supprimer le dernier élément de la liste */
Item *popLast(list_t *list) {
    if (list->numElements == 0) {
        return NULL;
    }

    Item *last = list->last;
    list->last = last->prev;
    if (list->last) {
        list->last->next = NULL;
    } else {
        list->first = NULL;
    }

    list->numElements--;
    return last;
}

/* Fonction pour supprimer un élément de la liste */
void delList(list_t *list, Item *node) {
    if (!list || !node)
        return;

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        list->first = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        list->last = node->prev;
    }

    list->numElements--;
    freeItem(node);
}

/* Fonction pour retourner et supprimer le meilleur élément avec la valeur f minimale */
Item *popBest(list_t *list) {
    if (!list || !list->first) {
        return NULL;
    }

    Item *bestnode = list->first;
    Item *current = list->first->next;

    while (current != NULL) {
        if (current->f < bestnode->f) {
            bestnode = current;
        }
        current = current->next;
    }

    delList(list, bestnode);
    return bestnode;
}

/* Fonction pour ajouter un élément au début de la liste */
void addFirst(list_t *list, Item *node) {
    if (list->numElements == 0) {
        list->first = node;
        list->last = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
    }
    list->numElements++;
}

/* Fonction pour ajouter un élément à la fin de la liste */
void addLast(list_t *list, Item *node) {
    if (!list || !node)
        return;

    if (list->numElements == 0) {
        list->first = node;
        list->last = node;
        node->next = NULL;
        node->prev = NULL;
    } else {
        node->next = list->first;
        node->prev = NULL;
        list->first->prev = node;
        list->first = node;
    }
    list->numElements++;
}

/* Fonction pour vider la liste et libérer les éléments */
void cleanupList(list_t *list) {
    if (!list)
        return;

    Item *current = list->first;
    while (current != NULL) {
        Item *next = current->next;
        freeItem(current);
        current = next;
    }

    list->first = NULL;
    list->last = NULL;
    list->numElements = 0;
}

/* Fonction pour afficher le contenu de la liste */
void printList(const list_t *list) {
    Item *item = list->first;
    while (item) {
        printf("%.2f [%s] - ", item->f, item->board);
        item = item->next;
    }
    printf(" (nb_items: %d)\n", list->numElements);
}
