#include "graph.h"

#include <stdlib.h>

Graph *newGraph()
{
    Graph *graph = calloc(1, sizeof(Graph*));
    graph->labels = newHashtable(100003);
    graph->nodeTable = calloc(10, sizeof(Node*));
    graph->nodeCount = 0;

    return graph;
}

void deleteGraph(Graph *graph)
{
    // TODO deleteGraph
}

Node *newNode()
{
    Node *node = calloc(1, sizeof(Node));
    node->edges = newList();

    return node;
}

bool addNode(Graph *graph, char *label) // TODO zastanowić się czy nie skopiować napisu przed dodaniem do hashtablicy. Bo ktoś może podmienić
{
    if(hashtableGet(graph->labels, label) == NULL)// TODO zastanowić się czy w hashtable insert trzeba sprawdzać
    {
        hashtableInsert(graph->labels, label, graph->nodeCount);
        graph->nodeTable[graph->nodeCount] = newNode();
        graph->nodeCount++;
    }
}

bool addEdge(Graph *graph, char *label1, char *label2, int length, int builtYear)
{
    // TODO list pushback
    int *v1 = hashtableGet(graph->labels, label1);
    int *v2 = hashtableGet(graph->labels, label2);

    Edge *newEdge = calloc(1, sizeof(Edge));
    newEdge->v1 = v1;
    newEdge->v2 = v2;
    newEdge->builtYear = builtYear;
    newEdge->length = length;

    listInsert(graph->nodeTable[*v1]->edges->end->prev, newEdge);
    listInsert(graph->nodeTable[*v2]->edges->end->prev, newEdge);
}