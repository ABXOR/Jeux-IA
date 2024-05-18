#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Item Functions */

Item *nodeAlloc()
{
	Item *node;

	node = (Item *)malloc(sizeof(Item));
	assert(node);

	node->board = NULL;
	node->parent = NULL;
	node->prev = NULL;
	node->next = NULL;
	node->f = node->g = node->h = (double)0.0;

	return node;
}

void freeItem(Item *node)
{
	if (node && node->board)
		free(node->board);
	if (node)
		free(node);
}

void initList(list_t *list_p)
{
	// if (list_p == NULL) list_p = malloc(sizeof(list_t));
	assert(list_p);

	list_p->numElements = 0;
	list_p->first = NULL;
	list_p->last = NULL;
}

int listCount(list_t *list)
{
	return list->numElements;
}

// return an item with corresponding board , or null
Item *onList(list_t *list, char *board)
{
	Item *test = list->first;
	while (test != NULL)
	{
		if (strcmp(test->board, board) == 0)
		{
			return test;
		}
		test = test->next;
	}

	return NULL;
}

// return and remove first item
Item *popFirst(list_t *list) //
{

	if (list->numElements == 0)
	{
		Item *item = NULL;
		return item;
	}
	if (list->numElements == 1)
	{
		Item *test = list->first;
		list->first = NULL;
		list->last = NULL;
		list->numElements -= 1;
		return test;
	}

	Item *test = list->first;
	list->first = list->first->next;
	list->numElements -= 1;
	return test;
}

// return and remove last item
Item *popLast(list_t *list) //
{
	if (list->numElements == 0)
	{
		Item *item = NULL;
		return item;
	}
	if (list->numElements == 1)
	{
		Item *test = list->last;
		list->first = NULL;
		list->last = NULL;
		list->numElements -= 1;
		return test;
	}
	Item *test = list->last;
	list->last = list->first->prev;
	list->numElements -= 1;
	return test;
}

// remove a node from list
void delList(list_t *list, Item *node)
{
	if (list == NULL || node == NULL)
		return;
	if (node == list->first)
	{
		list->first = node->next;
		if (list->first != NULL)
		{
			list->first->prev = NULL;
		}
	}
	else
	{
		node->prev->next = node->next;
	}
	if (node == list->last)
	{
		list->last = node->prev;
		if (list->last != NULL)
		{
			list->last->next = NULL;
		}
	}
	else
	{
		node->next->prev = node->prev;
	}
	list->numElements--;
	free(node);
}

// return and remove best item with minimal f value
Item *popBest(list_t *list) // and remove the best board from the list.
{
	if (list == NULL || list->first == NULL)
	{
		Item *item = NULL;
		return item;
	}
	Item *test = list->first;
	Item *bestnode = NULL;
	float bestF = list->first->f;
	while (test != NULL)
	{
		if (test->f < bestF)
		{
			bestF = test->f;
			bestnode = test;
		}
		test = test->next;
	}

	if (bestnode->prev != NULL)
		bestnode->prev->next = bestnode->next;
	else
		list->first = bestnode->next;

	if (bestnode->next != NULL)
		bestnode->next->prev = bestnode->prev;
	else
		list->last = bestnode->prev;

	list->numElements--;

	return bestnode;
}

// add item in top
void addFirst(list_t *list, Item *node) // add in head
{
	if (list->numElements == 0 || list->first == NULL)
	{
		list->first = node;
		list->last = node;
		list->numElements = 1;
		node->prev = NULL;
		node->next = NULL;
	}
	else
	{
		node->next = list->first;
		node->prev = NULL;
		list->first->prev = node;
		list->first = node;
		list->numElements += 1;
	}
}

// add item in queue
void addLast(list_t *list, Item *node) // add in tail
{
	if (list == NULL || node == NULL)
		return;

	if (list->first == NULL && list->last == NULL)
	{
		list->first = node;
		list->last = node;
		node->next = NULL;
		node->prev = NULL;
		list->numElements++;
	}
	else
	{
		node->next = list->first;
		node->prev = NULL;
		list->first->prev = node;
		list->first = node;
		list->numElements++;
	}
}
void cleanupList(list_t *list)
{
	if (list == NULL)
		return;

	Item *test = list->first;
	while (test != NULL)
	{
		Item *temp = test;
		test = test->next;

		free(temp);
	}

	list->first = NULL;
	list->last = NULL;
	list->numElements = 0;
}

void printList(list_t list)
{
	Item *item = list.first;
	while (item)
	{
		printf("%.2f [%s] - ", item->f, item->board);
		item = item->next;
	}
	printf(" (nb_items: %d)\n", list.numElements);
}

// TEST LIST
/*
int main()
{
	Item *item;
	char str[3];

	list_t openList;

	initList(&openList);

	for (int i = 0; i < 10; i++)
	{
		item = nodeAlloc();
		item->f = i;
		sprintf(str, "%2d", i);
		item->board = strdup(str);
		addLast(&openList, item);
	}

	for (int i = 20; i < 25; i++)
	{
		item = nodeAlloc();
		item->f = i;
		sprintf(str, "%2d", i);
		item->board = strdup(str);
		addFirst(&openList, item);
	}
	printf("Liste 1 de Base #1\n");
	printList(openList);
	printf("\n");

	Item *node = popBest(&openList);
	printf("best node = %.2f\n", node->f);
	printf("Liste pour ressortir le node avec le Best f\n");
	printList(openList);
	printf("\n");

	strcpy(str, "23");
	node = onList(&openList, str);
	if (node)
		printf("found %s: %.2f!\n", str, node->f);
	printf("");
	printList(openList);
	printf("\n");

	node = popFirst(&openList);

	item = nodeAlloc();
	item->f = 50;
	sprintf(str, "50");
	item->board = strdup(str);
	addLast(&openList, item);

	node = popFirst(&openList);
	if (node)
		printf("first: %.2f\n", node->f);
	printList(openList);
	printf("\n");

	node = popLast(&openList);
	if (node)
		printf("last: %.2f\n", node->f);
	printList(openList);
	printf("\n");

	printf("clean\n");
	cleanupList(&openList);
	printList(openList);
	printf("\n");

	return 0;
}
*/