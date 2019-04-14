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
} List;

List *newList(void);

void deleteList(List *list);

// wstawia element do listy po elem
bool listInsert(Element *elem, void *value);

#endif //DROGI_LIST_H
