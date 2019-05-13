#ifndef DROGI_VECTOR_H
#define DROGI_VECTOR_H

#include <stdbool.h>

typedef struct Vector
{
    void **tab;
    int size;
    int filled;
} Vector;

Vector *newVector();

void deleteVector();

bool vectorPushBack(Vector *vector, void *value);

#endif //DROGI_VECTOR_H
