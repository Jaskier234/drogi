#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>

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

List *newList(void);

void deleteList(List *list);

// wstawia element do listy po elem
bool listInsert(Element *elem, void *value);

bool listPushBack(List *list, void *value);

// O(n)
int listSize(List *list);

// TODO możnaby dodać funkcję getElementValue

#endif //DROGI_LIST_H
