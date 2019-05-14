#include "vector.h"

#include <stdlib.h>

const int INIT_SIZE = 16;

Vector *newVector()
{
    Vector *vector = calloc(1, sizeof(Vector));

    if(vector == NULL)
        return NULL;

    vector->tab = calloc(INIT_SIZE, sizeof(void*));

    if(vector->tab == NULL)
    {
        free(vector);
        return NULL;
    }

    vector->size = INIT_SIZE;
    vector->filled = 0;

    return vector;
}

void deleteVector(Vector *vector)
{
    if(vector == NULL)
        return;

    free(vector->tab);
    free(vector);
}

bool vectorPushBack(Vector *vector, void *value)
{
    if(vector->filled >= vector->size)
    {
        void **newVectorTab = realloc(vector->tab, vector->size * 2 * sizeof(void*));

        if(newVectorTab == NULL)
            return false;

        vector->tab = newVectorTab;
        vector->size *= 2;
    }

    vector->tab[vector->filled] = value; // TODO może w jednej linijce?
    vector->filled++;

    return true;
}

void vectorClear(Vector *vector)
{
    vector->filled = 0;
}