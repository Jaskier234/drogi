/**
 * @file
 * Interfejs grafu. //todo
 */

#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include "hashtable.h"
#include "list.h"
#include "vector.h"

const int INF;
const int minYear;
const int maxYear;

/**
 * Struktura reprezentująca wierzchołek grafu
 */
typedef struct Node
{
    List *edges; ///< Lista krawędzi  wychodzących z wierzchołka
    int *id; ///< Numer wierzchołka.
    bool visited; ///< Informacja o tym, czy wierzchołek został już odwiedzony
    ///< podczas wyszukiwania ścieżki
} Node;

/**
 * Struktura reprezentująca krawędź
 */
typedef struct Egde
{
    int v1, v2; ///< Indeksy wierzchołków
    int length; ///< Długość krawędzi
    int builtYear; ///< Rok budowy lub ostatniego remontu
} Edge;

/**
 * Struktura przechowująca wierzchołki i krawędzie pomiędzy nimi.
 */
typedef struct Graph
{
    Vector *nodes; ///< Tablica wskaźników na wierzchołki grafu
//    int tableSize; ///< Rozmiar tablicy nodes(ilość zaalokowanej pamięci).
    int nodeCount; ///< Liczba wierzchołków w grafie.
    List *ambiguous; ///< do bestPath
} Graph;

/**
 * Struktura przechowująca krawędź wraz z wyróżnionym wierzchołkiem
 */
typedef struct OrientedEdge
{
    int v; ///< Indeks wierzchołka, który jest wyróżniony
    Edge *edge; ///< Wskaźnik na krawędź
    bool isInPath; ///< Zmienna mówiąca, czy obiekt powinien zostać usunięty
    ///< po zakończeniu wyszukiwania ścieżki
} OrientedEdge;

/**
 * @brief Tworzy nowy graf.
 * @return Wskaźnik na graf lub NULL w przypadkum gdy nie udało się
 * zaalokować pamięci
 */
Graph *newGraph();

/**
 * @brief Zwalnia pamięć po grafie.
 * @param graph Wskaźnil na graf, który ma zostać usunięty. Gdy równe NULL
 * funkcja nic nie robi.
 */
void deleteGraph(Graph *graph);

//void deleteOrientedEdge(OrientedEdge *edge);

/**
 * @brief Dodaje wierzchołek do grafu.
 * @param graph Graf, do którego ma zostać dodany wierzchołek.
 * @return Wskaźnik na id wierzchołka lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
int *addNode(Graph *graph);

/**
 * @brief Dodaje krawędź do grafu pomiędzy wskazanymi wierzchołkami.
 * @param graph Graf, w którym zostanie dodana krawędź
 * @param v1 id wierzchołka do którego zostanie dodana krawędź.
 * @param v2 id wierzchołka do którego zostanie dodana krawędź.
 * @param length Długość krawędzi.
 * @param builtYear Rok budowy lub ostatniego remontu.
 * @return @p true jeśli dodawanie się powiedzie lub @p false, gdy krawędź już jest w grafie,
 * podano nieprawidłowe id wierzchołków lub nie udało się zaalokować pamięci.
 */
bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear);

/**
 * @brief Szuka w grafie krawędzi pomiędzy @p v1 i @p v2
 * @param graph Przeszukiwany graf.
 * @param v1 id wierzchołka
 * @param v2 id wierzchołka
 * @return Wskaźnik na krawędź, lub NULL jeśli parametry są niepoprawne lub
 * w grafie nie ma krawędzi pomiędzy @p v1 i @p v2
 */
Edge *getEdge(Graph *graph, int v1, int v2);

/**
 * @brief Usuwa krawędź z grafu.
 * Nie zwalnia pamięci po usuniętej krawędzi.
 * @param graph Graf, z którego zostanie usunięta krawędź
 * @param v1 id wierzchołka grafu
 * @param v2 id wierzchołka grafu
 * @return @p true jeśli krawędź została poprawnie usunięta lub @p false, gdy
 * nie usunięto krawędzi(np. w grafie jej nie było).
 */
bool removeEdge(Graph *graph, int v1, int v2);

List *bestPath(Graph *graph, int v1, int v2);

int min(int a, int b);

/**
 * @brief Funkcja wypisująca listę sąsiedztwa grafu na standardowe wyjście.
 * @param graph Wsgaźnik na graf, który ma zostać wypisany.
 */
void printGraph(Graph *graph);

#endif //DROGI_GRAPH_H
