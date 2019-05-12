#include "list.h"

#include <stdlib.h>

List *newList(Memory *memory)
{
    List *list;
    Element *beginElem;
    Element *endElem;

    if(memory == NULL)
    {
        list = calloc(1, sizeof(List));
        beginElem = calloc(1, sizeof(Element));
        endElem = calloc(1, sizeof(Element));
    }
    else
    {
        list = getMemory(memory, sizeof(List));
        beginElem = getMemory(memory, sizeof(Element));
        endElem = getMemory(memory, sizeof(Element));
    }

    if(list == NULL || beginElem == NULL || endElem == NULL)
    {
        if(memory == NULL) // jeśli używamy memory, pamięć zostanie zwolniona później
        {
            free(list);
            free(beginElem);
            free(endElem);
        }
        return NULL;
    }

    beginElem->next = endElem;
    endElem->prev = beginElem;


    list->begin = beginElem;
    list->end = endElem;

    return list;
}

void deleteList(List *list, bool del)
{
    if(list == NULL)
        return;

    Element *iter = list->begin;

    do
    {
        Element *next = iter->next;

        if(del)
            free(iter->value);

        free(iter);
        iter = next;
    }
    while(iter != list->end);

    free(list->end);

    free(list);
}

bool listInsert(Element *elem, void *value, Memory *memory)
{
    Element *newElem;
    if(memory == NULL)
        newElem = calloc(1, sizeof(Element));
    else
        newElem = getMemory(memory, sizeof(Element));

    if(newElem == NULL)
        return false;

    Element *next = elem->next;

    elem->next = newElem;
    newElem->next = next;

    next->prev = newElem;
    newElem->prev = elem;

    newElem->value = value;

    return true;
}

bool listRemove(Element *elem)
{
    if(elem->next == NULL || elem->prev == NULL)
        return false;

    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    free(elem);

    return true;
}

int listSize(List *list)
{
    if(list == NULL)
        return 0;

    int size = 0;
    foreach(it, list)
    {
        size++;
    }
    return size;
}

bool listPushBack(List *list, void *value, Memory *memory)
{
    return listInsert(list->end->prev, value, memory);
}

void listInsertList(Element *elem, List *list)
{
    Element *next = elem->next;

    elem->next = list->begin->next;
    list->end->prev->next = next;

    next->prev = list->end->prev;
    list->begin->next->prev = elem;

    list->begin->next = list->end;
    list->end->prev = list->begin;
}
