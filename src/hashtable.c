#include "hashtable.h"

#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct Hashtable
{
    int MOD;
    List **table;
} Hashtable;

typedef struct Pair
{
    void *key, *value;
} Pair;

Pair *newPair(void *key, void *value)
{
    Pair *pair = calloc(1,sizeof(Pair));
    pair->key = key;
    pair->value = value;

    return pair;
}

Hashtable *newHashtable()
{
    Hashtable *hashtable = calloc(1, sizeof(Hashtable));

    hashtable->MOD = 1000003;
    hashtable->table = calloc(hashtable->MOD, sizeof(List*));

    return hashtable;
}

void deleteHashtable(Hashtable *self)
{
    for(int i=0; i<self->MOD; i++)
        if(self->table[i] != NULL)
            deleteList(self->table[i]); // TODO elem->value wycieka. dodać funkcję zwalniającą???

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

bool hashtableInsert(Hashtable *self, char *key, void *value)
{
    int index = hash(self,key);

    if(self->table[index] == NULL)
        self->table[index] = newList();

    listInsert(self->table[index]->end->prev, newPair(key, value));

    return true;
}

void *get(Hashtable *self, char *key)
{
    int index = hash(self, key);

    Element *elem = self->table[index]->begin->next;

    while(elem->next != NULL && strcmp(key, ((Pair *)elem->value)->key) != 0)
        elem = elem->next;

    if(elem->next != NULL)
        return ((Pair *)elem->value)->value;// TODO naprawić kolizję nazw value
    else
        return NULL;
}