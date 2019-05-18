/** @file
 * Interfejs modułu parsującego wejście
 */

#ifndef valid_h
#define valid_h

#include "vector.h"

const char *ADD_ROAD; ///< stała używana do parsowania wejścia.
const char *REPAIR_ROAD; ///< stała używana do parsowania wejścia.
const char *GET_ROUTE_DESRIPTION; ///< stała używana do parsowania wejścia.
const char *ERROR; ///< stała używana do parsowania wejścia.

/**
 * Sprawdza, czy podana na wejściu linia jest poprawnym poleceniem oraz
 * konwertuje napis na wektor, który jest dalej przetwarzany.
 * Sprawdzana jest liczba parametrów, poprawność nazw miast i poprawność
 * liczb.
 * @param input wskaźnik na wczytaną linię
 * @param args Wskaźnik na wektor, do którego zostaną zapisane parametry
 * @return Liczba całkowita z przedziału [0,2].
 * 0 - Niepoprawna linia
 * 1 - Poprawna linia
 * 2 - Ignorowana linia
 */
int correct(char *input, Vector *args);

/**
 * Sprawdza, czy podany napis jest poprawną nazwą miasta.
 * Poprawną nazwą miasta jest niepusty napis niezawierający znaków ASCII o
 * kodach z mniejszych niż 32 oraz średnika.
 * @param cityName Wskaźnik na sprawdzaną nazwę miasta.
 * @return @p true gdy podany napis jest poprawną nazwą miasta lub @p false
 * w przeciwnym wypadku.
 */
bool isNameCorrect(const char *cityName);

/**
 * Sprawdza czy podany napis jest poprawną liczbą.
 * Zera wiodące są ignorowane.
 * @param number Wskaźnik na napis, który jest sprawdzany.
 * @param maxNumber Wskaźnk na napis będący limitem wielkości liczby.
 * Jeśli podana liczba będzie większa, funkcja zwróci @p false.
 * @param isSigned Jeśli @p true, funkcja akceptuje również liczby ujemne.
 * Wtedy liczba na wejściu jest akceptowana, jeśli jest w
 * przedziale [-maxNumber, maxNumber]
 * @return @p true jeśli podany napis jest poprawną liczbą lub @p false
 * w przeciwnym wypadku.
 */
bool correctInt(char *number, const char *maxNumber, bool isSigned);

#endif // valid_h
