#ifndef DROGI_GRAPH_H
#define DROGI_GRAPH_H

#include "hashtable.h"
#include "list.h"

typedef struct Node
{
    List *edges;
} Node;

typedef struct Egde
{
    int v1, v2;
    int length;
    int builtYear;
} Edge;

typedef struct Graph
{
    Hashtable *labels;
    Node **nodeTable;
    int nodeCount;
} Graph;

bool addNode(Graph *graph, char *label);

bool addEdge(Graph *graph, char *label1, char *label2, int length, int builtYear);

#endif //DROGI_GRAPH_H