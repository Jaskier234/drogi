#ifndef DROGI_HASHTABLE_H
#define DROGI_HASHTABLE_H

#include <stdbool.h>

typedef struct Hashtable Hashtable;

Hashtable *newHashtable(void);

void deleteHashtable(Hashtable *self);

// TODO zmienić typ key, value na ogólny
// TODO czy self to na pewno dobry pomysł? Nie zmienić, na inne

// zwraca false jeśli nie uda się dodać
bool hashtableInsert(Hashtable *self, char *key, void *value);

void *get(Hashtable *self, char *key);

#endif //DROGI_HASHTABLE_H
