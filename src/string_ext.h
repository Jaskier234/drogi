/** @file
 * Interfejs modułu obsługującego operacje na napisach.
 */

#ifndef DROGI_STRING_EXT_H
#define DROGI_STRING_EXT_H

#include <string.h>
#include <inttypes.h>
#include "vector.h"

int lineNr; ///< Liczba wczytanych linii.

/**
 * Konwertuje liczbę na napis.
 * @param a Liczba do skonwertowania.
 * @return Wskaźnik na napis.
 */
char *intToString(int64_t a);

/**
 * Konkatenuje dwa stringi.
 * @param string1 Napis, do którego zostanie doklejony drugi napis.
 * @param string2 Doklejany napis.
 * @param size Wskaźnik na rozmiar pierwszego napisu.
 * @param allocated Wskaźnik na rozmiar zaalokowanego miejsca na pierwszy napis.
 * @return Wskaźnik na skonkatenowany napis.
 */
char *concatenate(char *string1, const char *string2, int *size, int *allocated);

/**
 * Parsuje wejście do następnej poprawnej komendy.
 * Zwrócony wektor powinien zostać usunięty. Pamięć użyta do wczytania
 * linii również powinna być zwolniona. Wskaźnik na nią zawsze jest równy
 * vector->tab[0]
 * @return Zwraca wskaźnik na wektor z parametrami lub NULL, gdy nie ma
 * więcej komend na wejściu lub nie udało się zaalokować pamięci.
 */
Vector *nextCommand();

/**
 * Konwertuje napis na liczbę.
 * Nie sprawdza, czy podany napis jest poprawną liczbą.
 * @param string Napis, który zostanie skonwertowany.
 * @return Skonwertowana liczba.
 */
int64_t stringToNum(char *string);

#endif //DROGI_STRING_EXT_H
