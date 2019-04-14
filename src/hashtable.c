#include "hashtable.h"

#include <stdlib.h>

typedef struct Hashtable
{
    int MOD;      // narazie nie obsługuje kolizji
    char **table; // TODO zmienić typ na listę typu void*
} Hashtable;

Hashtable *newHashtable()
{
    Hashtable *hashtable = calloc(1, sizeof(Hashtable));

    hashtable->MOD = 1000003;
    hashtable->table = calloc(hashtable->MOD, sizeof(char*));

    return hashtable;
}

void deleteHashtable(Hashtable *self)
{
    free(self->table);
    free(self);
}

int hash(Hashtable *self, char *string)
{
    int hashValue = 0;
    int exp = 1;
    while(*string != 0)
    {
        hashValue += (*string) * exp;
        hashValue %= self->MOD;
        exp *= 31;
        exp %= self->MOD;
        string++;
    }

    return hashValue;
}

bool insert(Hashtable *self, char *key, char *value)
{
    int index = hash(self,key);
    self->table[index] = value;

    return true;
}

char *get(Hashtable *self, char *key)
{
    int index = hash(self, key);
    return self->table[index];
}