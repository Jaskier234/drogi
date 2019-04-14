#include "list.h"

#include <stdlib.h>

List *newList()
{
    List *list = calloc(1, sizeof(List));
    Element *beginElem = calloc(1, sizeof(Element));
    Element *endElem = calloc(1, sizeof(Element));

    beginElem->next = endElem;
    endElem->prev = beginElem;

    list->begin = beginElem;
    list->end = endElem;
    list->size = 0;

    return list;
}

void deleteList(List *list)
{
    Element *iter = list->begin;

    // zakłada, że struktura pod value jest zwolniona
    while(iter != list->end)
    {
        Element *next = iter->next;
        // free(iter->value); użytkownik powinien zwolnić wszystkie pola value przed wywołaniem deleteList()
        free(iter);
        iter = next;
    }
    free(list->end);

    free(list);
}

bool listInsert(Element *elem, void *value)
{
    // TODO ewentualnie zaimplementować moduł do zarządznia pamiecią
    Element *newElem = calloc(1, sizeof(Element));
    // TODO sprawdzanie czy się udało

    Element *next = elem->next;

    elem->next = newElem;
    newElem->next = next;

    next->prev = newElem;
    newElem->prev = elem;

    newElem->value = value;


    return true;
}
