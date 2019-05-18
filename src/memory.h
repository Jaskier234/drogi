/**
 * @file
 * Interfejs modułu zarządzającego pamięcią.
 * Moduł alokuje większe fragmenty pamięci, które potem są rozdzielane
 * pomiędzy inne moduły. Struktura memory zawiera tablicę tablic. Każda
 * kolejna tablica jest dwukrotnie większa niż poprzednia. Gdy w aktualnej
 * tablicy skończy się pamięć, alokowana jest następna.
 */

#ifndef DROGI_MEMORY_H
#define DROGI_MEMORY_H

#include <stddef.h>

/**
 * Struktura reprezentująca obiekt memory.
 */
typedef struct Memory Memory;

/**
 * Tworzy nowy obiekt memory.
 * @return Wskaźnik na memory lub NULL, gdy nie udało się zaalokować pamięci.
 */
Memory *newMemory();

/**
 * Zwraca wskźnik na pamięć, która może być użyta przez inne struktury.
 * @param memory Obiekt z którego zostanie przydzielona pamięć.
 * @param size rozmiar przydzielonej pamięci w bajtach.
 * @return Wskaźnik na przydzielony obszar pamięci.
 */
void *getMemory(Memory *memory, size_t size);

/**
 * Zwalnia całą pamięć zaalokowaną w obiekcie memory.
 * @param memory Obiekt memory, który ma zostać zwolniony.
 */
void deleteMemory(Memory *memory);

#endif //DROGI_MEMORY_H
