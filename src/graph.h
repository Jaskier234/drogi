#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include "hashtable.h"
#include "list.h"

const int INF;
const int minYear;
const int maxYear;

typedef struct Node
{
    List *edges;
    int *id;
    bool visited;
    char *label;
} Node;

typedef struct Egde
{
    int v1, v2;
    int length;
    int builtYear;
} Edge;

typedef struct Graph
{
    Node **nodeTable;
    int tableSize;
    int nodeCount;
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

int *addNode(Graph *graph, const char *label);

bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear);

Edge *getEdge(Graph *graph, int v1, int v2);

void removeEdge(Graph *graph, int v1, int v2);

List *bestPath(Graph *graph, int v1, int v2);

int min(int a, int b);

#endif //DROGI_GRAPH_H
