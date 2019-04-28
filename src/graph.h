#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include <limits.h>
#include "hashtable.h"
#include "list.h"

const int INF = INT_MAX;
const int minYear = -2000;
const int maxYear = 2100;

typedef struct Node // TODO dodać label
{
    List *edges;
    int *id;
    bool visited;
} Node;

typedef struct Egde
{
    int v1, v2;
    int length;
    int builtYear;
} Edge;

typedef struct Graph
{
//    Hashtable **labels;
    Node **nodeTable;
    int nodeCount;
} Graph;

typedef struct OrientedEdge
{
    int v;
    Edge *edge;
} OrientedEdge;

Graph *newGraph();

void deleteGraph(Graph *graph);

int * addNode(Graph *graph);

bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear);

bool isInGraph(Graph *graph, int v);

Edge *getEdge(Graph *graph, int v1, int v2);

void removeEdge(Graph *graph, int v1, int v2);

List *bestPath(Graph *graph, int v1, int v2);

int min(int a, int b);

#endif //DROGI_GRAPH_H
