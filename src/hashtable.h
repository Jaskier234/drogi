#ifndef DROGI_HASHTABLE_H
#define DROGI_HASHTABLE_H

#include <stdbool.h>
#include "memory.h"

typedef struct Hashtable Hashtable;

Hashtable **newHashtable(int size, Memory *memory);

void deleteHashtable(Hashtable **hash);

// TODO zmienić typ key, value na ogólny
// TODO zmienić nazwę self na inną

// zwraca false jeśli nie uda się dodać
bool hashtableInsert(Hashtable **self, char *key, void *value);

void *hashtableGet(Hashtable **self, const char *key);

int maxListLength(Hashtable **hash);

#endif //DROGI_HASHTABLE_H
