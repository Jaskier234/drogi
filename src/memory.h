#ifndef DROGI_MEMORY_H
#define DROGI_MEMORY_H

#include "list.h"

typedef struct Memory
{
    Element **content; // TODO lepsza nazwa
    int size;
    int filled;
} Memory;

Memory *newMemory();

Element *getMemory(Memory *memory);

#endif //DROGI_MEMORY_H
