/** @file
 * Interfejs dwukierunkowej listy generycznej
 */

#ifndef DROGI_LIST_H
#define DROGI_LIST_H

#include <stdbool.h>
#include "memory.h"

// TODO memory w dokumentacji

/**
 * Pętla przechodząca po wszystkich elementach listy
 */
#define foreach(it, list) for(Element *it = list->begin->next; it != list->end; it = it->next)

/**
 * Struktura będąca elementem listy
 */
typedef struct Element
{
    struct Element *prev; ///< Wskaźnik na następny element listy.
    struct Element *next; ///< Wskaźnik na poprzedni element listy.
    void *value; ///< Wskaźnik na obiekt przechowywany w liście.
} Element;

/**
 * Struktura listy
 */
typedef struct List
{
    Element *begin; ///< Pusty element na początku listy pełniący rolę wartowników
    Element *end; ///< Pusty element na końcu listy pełniący rolę wartowników
} List;

/**
 * @brief Tworzy nową listę.
 * Alokuje pamięć, tworzy nową listę.
 * @param memory możliwe że usunę
 * @return Wskaźnik na utworzoną listę lub NULL, gdy nie uda się zaalokować pamięci
 */
List *newList(Memory *memory);

/**
 * @brief Zwalnia pamięć po liście.
 * @param list Wskaźnik na usuwaną listę.
 * @param del Jeśli jest true, to przy usuwaniu listy zostanie zwolniona
 * pamięć pod wskaźnikiem przechowywanym w liście(@p value). Jeśli false zostanie zwolniona
 * tylko pamięć zaalokowana na listę.
 */
void deleteList(List *list, bool del);

/**
 * @brief Wstawia do listy element po @p elem
 * @param elem Element po którym zostanie wstawiony element
 * @param value Wskaźnik na strukturę, która ma być wstawiona do listy
 * @param memory możliwe że usunę
 * @return Zwraca @p true jeśli uda się dodać element lub @p false
 */
bool listInsert(Element *elem, void *value, Memory *memory);

/**
 * @brief Usuwa element @p elem z listy.
 * Zwalnia tylko pamięć zaalokowaną na Element listy. Pamięć pod @p elem->value
 * nie jest zwalniana.
 * @param elem Element do usunięcia.
 * @return Zwraca @p true, gdy uda się usunąć element. Gdy parametr jest wartownikiem
 * zwraca @p false i nic nie robi.
 */
bool listRemove(Element *elem);

/**
 * @brief Wstawia element na koniec listy.
 * @param list Lista, do której będzie wstawiany element.
 * @param value Wskaźnik na strukturę wstawianą do listy.
 * @param memory chyba usunę
 * @return Zwraca @p true, gdy uda się wstawić element lub @p false
 * w przeciwnym wypadku.
 */
bool listPushBack(List *list, void *value, Memory *memory);

/**
 * @brief Do listy zawierającej @p elem wstawia wszystkie elementy listy @p list. Elementy są wstawiane po elemencie @p elem
 * w takiej kolejności w jakiej były w pierwszej liście.
 * Po użyciu tej funkcji @p list jest pusta(zawiera tylko wartowników).
 * @param elem Element, po którym będą wstawione elementy listy @p list.
 * @param list Lista, krórej elementy zostaną wstawione. Po wywołaniu funkcji staje się pusta.
 */
void listInsertList(Element *elem, List *list);

/**
 * @brief Liczy rozmiar listy.
 * Czas działania tej funkcji jest proporcjonalny to rozmiaru listy.
 * @param list Lista, której długość ma zostać obliczona.
 * @return Rozmiar listy.
 */
int listSize(List *list);

#endif //DROGI_LIST_H
