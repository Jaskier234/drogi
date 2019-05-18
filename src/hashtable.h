/** @file
 * Interfejs tablicy haszującej. Tablica obsługuje operacje wstawiania
 * i pobierania wartości pod dany klucz. Klucz powinien być napisem w stylu C
 * wastość może być dowolnym wskaźnikiem. Tablica korzysta z modułu memory.
 */

#ifndef DROGI_HASHTABLE_H
#define DROGI_HASHTABLE_H

#include <stdbool.h>

#include "memory.h"

/**
 * Struktura tablicy haszującej
 */
typedef struct Hashtable Hashtable;

/**
 * Tworzy nową tablicę haszującą.
 * @param size Początkowy rozmiar tablicy.
 * @param memory Wskaźnik na obiekt memory, z którego ma korzystać tablica.
 * Można przekazać NULL. Wtedy zostanie utworzony nowy obiekt memory
 * z którego tablica będzie korzystać
 * @return Wskaźnik na wskaźnik na tablicę haszującą lub NULL gdy nie udało
 * się zaalokować pamięci.
 */
Hashtable **newHashtable(int size, Memory *memory);

/**
 * Usuwa tablicę haszującą.
 * @param hash Wskaźnik na tablicę, która ma zostać usunięta.
 */
void deleteHashtable(Hashtable **hash);

/**
 * Dodaje do tablicy wartość.
 * @param self Wskaźnik na tablicę, do której ma zostać dodana wartość.
 * @param key Wskaźnik na napis będący kluczem, pod którym zostanie zapisane @p value.
 * @param value Wskaźnik, który zostanie dodany do tablicy
 * @return @p true, gdy udało się dodać lub @p false gdy nie udało się
 * zaalokować pamięci.
 */
bool hashtableInsert(Hashtable **self, char *key, void *value);

/**
 * Zwraca wartość dodaną pod podanym kluczem.
 * @param self Wskaźnik na tablicę, z której wartość powinna zostać pobrana.
 * @param key Wskaźnik na klucz, spod którego wartość ma zostać zwrócona.
 * @return Wskaźnik na wartość, która była pod kluczem @p key lub NULL gdy
 * w tablicy nie było wartości pod danym kluczem.
 */
void *hashtableGet(Hashtable **self, const char *key);

/**
 * Zwraca długość najdłuższej listy w tablicy.
 * @param hash Wskaźnik na tablicę haszującą.
 * @return długość najdłuższej listy w tablicy.
 */
int maxListLength(Hashtable **hash);

#endif //DROGI_HASHTABLE_H
