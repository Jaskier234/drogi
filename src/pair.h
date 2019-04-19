#ifndef DROGI_PAIR_H
#define DROGI_PAIR_H

typedef struct Pair
{
    void *key, *value;
} Pair;

Pair *newPair(void *key, void *value);

#endif //DROGI_PAIR_H
