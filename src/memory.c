#include "memory.h"

#include <stdlib.h>

Memory *newMemory()
{
    Memory *memory = calloc(1, sizeof(Memory));

    memory->content = calloc(16, sizeof(Element));
    memory->size = 16;
    memory->filled = 0;

    return memory;
}

Element *getMemory(Memory *memory)
{
    if(memory->filled >= memory->size)
    {
        memory->content = realloc(memory->content, memory->size*2);
    }

    memory->filled++;
    return memory->content[memory->filled-1];
}