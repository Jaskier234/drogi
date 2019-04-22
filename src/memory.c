#include "memory.h"

#include <stdlib.h>

typedef struct Memory
{
    void ***content; // TODO lepsza nazwa
    unsigned int currentIndex;
    unsigned int size;
    unsigned int filled;
} Memory;

Memory *newMemory()
{
    const size_t initialSize = 64;

    Memory *memory = calloc(1, sizeof(Memory));

    memory->content = calloc(30, sizeof(void**));
    memory->currentIndex = 0;
    memory->content[memory->currentIndex] = calloc(initialSize, sizeof(void*));

    memory->size = initialSize;
    memory->filled = 0;

    return memory;
}

void *getMemory(Memory *memory, size_t size)
{
    while(memory->filled + size >= memory->size)
    {
        memory->size *= 2;
        memory->filled = 0;
        memory->currentIndex++;
        memory->content[memory->currentIndex] = calloc(memory->size, sizeof(void*));
    }

    if(memory->content[memory->currentIndex] == NULL)
        return NULL;

    memory->filled += size;
    return &memory->content[memory->currentIndex][memory->filled-size];
}