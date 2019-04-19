#include "pair.h"

#include <stdlib.h>

Pair *newPair(void *key, void *value)
{
    Pair *pair = calloc(1,sizeof(Pair));
    pair->key = key;
    pair->value = value;

    return pair;
}

void deletePair(); //TODO usuwanie
// TODO ten sam problem z usuwaniem co w liście