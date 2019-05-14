/**
 * @file
 * Interfejs modułu obsługującego operacje na napisach.
 */

#ifndef DROGI_STRING_EXT_H
#define DROGI_STRING_EXT_H

#include <string.h>
#include <inttypes.h>
#include "vector.h"

/**
 * Konwertuje liczbę na napis.
 * @param a Liczba do skonwertowania.
 * @return Wskaźnik na napis.
 */
char *intToString(int a);

/**
 * Konkatenuje dwa stringi.
 * @param string1 Napis, do którego zostanie doklejony drugi napis.
 * @param string2 Doklejany napis.
 * @param size Wskaźnik na rozmiar pierwszego napisu.
 * @param allocated Wskaźnik na rozmiar zaalokowanego miejsca na pierwszy napis.
 * @return Wskaźnik na skonkatenowany napis.
 */
char *concatenate(char *string1, const char *string2, int *size, int *allocated);

// Returns next correct command or NULL when there is no more commands
Vector *nextCommand();

// Converts string to uint64_t
uint64_t stringToNum(char *string);

#endif //DROGI_STRING_EXT_H
