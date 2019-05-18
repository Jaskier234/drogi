/** @file
 * Interfejs kolejki priorytetowej.
 */

#ifndef DROGI_PRIORITY_QUEUE_H
#define DROGI_PRIORITY_QUEUE_H

#include <stdbool.h>

/**
 * Struktura reprezentująca kolejkę priorytetową
 */
typedef struct PriorityQueue
{
    void **content; ///< Tbalica przechowująca elementy kolejki.
    int size; ///< Rozmiar zaalokowanej pamięci.
    int filled; ///< Liczba elementów w kolejce.
    int(*compare)(void*, void*); ///< Funkcja porównująca elementy kolejki.
    ///< Powinna zwracać 0 gdy są równe, 1 - gdy pierwszy argument jest
    ///< mniejszy lub -1 w przeciwnym wypadku.
} PriorityQueue;

/**
 * Tworzy nową kolejkę priorytetową.
 * @param cmp Funkcja porównująca elementy.
 * @return Wskaźnik na kolejkę lub NULL gdy nie uda się zaalokować pamięci.
 */
PriorityQueue *newPriorityQueue(int(*cmp)(void *, void *));

/**
 * Usuwa kolejkę priorytetową.
 * @param queue Wskaźnik na usuwaną kolejkę
 */
void deletePriorityQueue(PriorityQueue *queue);

/**
 * Dodaje element do kolejki.
 * @param queue Wskaźnik na kojejkę, do której dodawany jest element.
 * @param elem Wskaźnik na element.
 * @return @p true gdy udało się dodać element lub @p false gdy nie udało się
 * zaalokować pamięci.
 */
bool priorityQueuePush(PriorityQueue *queue, void *elem);

/**
 * Usuwa najmniejszy element z kolejki i zwraca go.
 * @param queue Wskażnik na kolejkę, z której jest usuwany element.
 * @return Wskaźnik na usunięty element lub NULL jeśli kolejka jest pusta.
 */
void * priorityQueuePop(PriorityQueue *queue);

/**
 * Sprawdza, czy kolejka jest pusta.
 * @param queue Wskaźnik na sprawdzaną kolejkę.
 * @return @p true jeśli kolejka jest pusta lub @p false w przeciwnym wypadku.
 */
bool isEmpty(PriorityQueue *queue);

#endif //DROGI_PRIORITY_QUEUE_H
