#include "memory.h"

#include <stdlib.h>

typedef struct Memory
{
    void ***content; ///< tablica przechowująca zaalokowaną pamięć. Jest to
    ///< tablica tablic. Kolejna tablica jest alokowana z 2 razy większą ilością
    ///< miejsca co poprzednia.
    unsigned int currentIndex; ///< Indeks tablicy, która jest obecnie używana.
    unsigned int size; ///< Ilość zaalokowanej pamięci w aktualnie używanej tablicy.
    unsigned int filled; ///< Ilość zużytej pamięci w aktualnie używanej tablicy.
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

void deleteMemory(Memory *memory)
{
    for(int i = 0; i < 30; i++)
        free(memory->content[i]);

    free(memory->content);
    free(memory);
}