#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include "hashtable.h"
#include "list.h"

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
    bool visited; // TODO może przenieść to do dijkstry// chyba jednak nie
//    char *label;
} Node;

/**
 * Struktura reprezentująca krawędź
 */
typedef struct Egde
{
    int v1, v2;
    int length;
    int builtYear;
} Edge;

/**
 * Struktura przechowująca wierzchołki i krawędzie pomiędzy nimi.
 */
typedef struct Graph
{
    Node **nodes; ///< Tablica wskaźników na wierzchołki grafu
    int tableSize; ///< Rozmiar tablicy nodes(ilość zaalokowanej pamięci).
    int nodeCount; ///< Liczba wierzchołków w grafie.
} Graph;

typedef struct OrientedEdge
{
    int v;
    Edge *edge;
    bool isInPath;
} OrientedEdge;

Graph *newGraph();

void deleteGraph(Graph *graph);

void deleteOrientedEdge(OrientedEdge *edge);

int *addNode(Graph *graph);

bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear);

Edge *getEdge(Graph *graph, int v1, int v2);

bool removeEdge(Graph *graph, int v1, int v2);

List *bestPath(Graph *graph, int v1, int v2);

int min(int a, int b);

void printGraph(Graph *graph);

#endif //DROGI_GRAPH_H
