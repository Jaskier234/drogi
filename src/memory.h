#ifndef DROGI_MEMORY_H
#define DROGI_MEMORY_H

#include <stddef.h>

typedef struct Memory Memory;

Memory *newMemory();

void *getMemory(Memory *memory, size_t size);

void deleteMemory(Memory *memory);

#endif //DROGI_MEMORY_H
