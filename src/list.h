#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>
#include "memory.h"

typedef struct Element
{
    struct Element *prev, *next;
    void *value;
} Element;

typedef struct List
{
    Element *begin, *end;
    int size;
} List;

List *newList(Memory *memory);

void deleteList(List *list);

// wstawia element do listy po elem
bool listInsert(Element *elem, void *value, Memory *memory);

bool listRemove(Element *elem);

bool listPushBack(List *list, void *value, Memory *memory);

// O(n)
int listSize(List *list);

#endif //DROGI_LIST_H
