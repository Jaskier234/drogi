/**
 * @file
 * Interfejs grafu. Graf obsługuje operacje dodawania wierzchołków oraz
 * dodawania i usuwania krawędzi.
 * Nie można dodawać wielokrotnych krawędzi.
 */

#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include "list.h"
#include "vector.h"

const int INF; ///< Infinity
const int minYear; ///< Minimalny możliwy rok budowy drogi
const int maxYear; ///< Maksymalny możliwy rok budowy drogi

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
    int v1; ///< Indeks wierzchołka
    int v2; ///< Indeks wierzchołka
    unsigned length; ///< Długość krawędzi
    int builtYear; ///< Rok budowy lub ostatniego remontu
} Edge;

/**
 * Struktura przechowująca wierzchołki i krawędzie pomiędzy nimi.
 */
typedef struct Graph
{
    Vector *nodes; ///< Tablica wskaźników na wierzchołki grafu
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

/**
 * Tworzy nowy obiekt OrientedEdge.
 * @param edge Wsjaźnik na krawędź.
 * @param v Indeks wyróżnionego wierzchołka.
 * @return Wskaźnik na OrientedEdge.
 */
OrientedEdge *newOrientedEdge(Edge *edge, int v);

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
 * @brief Zwraca wierzchołek do którego prowadzi krawędź @p edge z wierzchołka @p v.
 * @param edge Rozpatrywana krawędź.
 * @param v id wierzchołka początkowego
 * @return Id wierzchołka do którego prowadzi krawędź.
 */
int otherNodeId(Edge *edge, int v);

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

/**
 * @brief Porównuje dwie krawędzie.
 * @param edge1 Krawędź, która będzie porównywana.
 * @param edge2 Krawędź, która będzie porównywana.
 * @return @p true jeśli krawędzie są równe lub @p false, gdy są różne.
 */
bool eqEdges(Edge *edge1, Edge *edge2);

/**
 * Wyszukuje najkrótszą ścieżkę pomiędzy wierzchołkmi o id v1 i v2. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param graph Wskaźnik na graf, w którym są wierzchołki
 * @param v1 Indeks wierzchołka w grafie
 * @param v2 Indeks wierzchołka w grafie
 * @return Wskażnik na listę elementów OrientedEdge, reprezentującą ścieżkę,
 * NULL w przypadku, gdy nie udało się zaalokować pamięci, lub wskaźnik na
 * @p graph->ambiguous, gdy nie udało się jednoznacznie wyznaczyć ścieżki.
 */
List *bestPath(Graph *graph, int v1, int v2);

/**
 * Funkcja minimum.
 * @param a Liczba całkowita.
 * @param b Liczba całkowita.
 * @return Zwraca mniejszą z dwóch liczb
 */
int min(int a, int b);

/**
 * @brief Funkcja wypisująca listę sąsiedztwa grafu na standardowe wyjście.
 * @param graph Wsgaźnik na graf, który ma zostać wypisany.
 */
void printGraph(Graph *graph);

#endif //DROGI_GRAPH_H
