/**
 * @file Interfejs generycznego wektora, przechowującego wskażniki
 * na struktury dowolnego typu.
 */

#ifndef DROGI_VECTOR_H
#define DROGI_VECTOR_H

#include <stdbool.h>

typedef struct Vector
{
    void **tab; ///< Tablica przechowująca dane.
    int size; ///< Ilość zaalokowanej pamięci.
    int filled; ///< Ilość zajętej pamięci.
} Vector;

/**
 * Tworzy w pamięci nowy wektor.
 * @return Wskaźnik na wektor lub NULL, gdy nie udało się zaalokować pamięci.
 */
Vector *newVector();

/**
 * Zwalnia pamięć po wektorze. Przed wywołaniem tej funkcji powinny zostać
 * zwolnione struktury przechowywane w wektorze.
 * @param vector Wektor, który ma zostać zwolniony.
 */
void deleteVector(Vector *vector);

/**
 * Dodaje strukturę do wektora.
 * @param vector Wektor, do którego zostanie dodana struktura.
 * @param value Wskaźnik na dodawaną strukturę.
 * @return @p true gdy udało się dodać strukturę do wektora lub @p false, gdy
 * nie udało się zaalokować pamięci.
 */
bool vectorPushBack(Vector *vector, void *value);

/**
 * usuwa
 * @param vector
 */
void vectorClear(Vector *vector);

#endif //DROGI_VECTOR_H
