#include "graph.h"

#include <stdlib.h>

Graph *newGraph()
{
    Graph *graph = calloc(1, sizeof(Graph*));
    graph->labels = newHashtable(16, NULL);
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
    node->edges = newList(NULL);
    node->id = calloc(1,sizeof(int));
    return node;
}

int addNode(Graph *graph, char *label) // TODO zastanowić się czy nie skopiować napisu przed dodaniem do hashtablicy. Bo ktoś może podmienić
{
    if(!isInGraph(graph, label))// TODO getHashtable + insert (2 przejścia niepotrzebnie)
    {
        graph->nodeTable[graph->nodeCount] = newNode();
        *graph->nodeTable[graph->nodeCount]->id = graph->nodeCount;
        hashtableInsert(graph->labels, label, graph->nodeTable[graph->nodeCount]->id);
        graph->nodeCount++;
        // TODO resize nodetable
        return
    }

    return -1; // TODO ustalić kody błędów
    // wierzchołek istnieje
    // nie udało się zaalokować pamięci
}

bool addEdge(Graph *graph, char *label1, char *label2, int length, int builtYear)
{
    // TODO kilka zapytań do hashmapy o to samo(może jakaś fajna funkcja)
    // TODO sprawdzić czy nie istnieje już taka krawędź
    addNode(graph, label1);
    addNode(graph, label2);

    int *v1 = hashtableGet(graph->labels, label1);
    int *v2 = hashtableGet(graph->labels, label2);

    Edge *newEdge = calloc(1, sizeof(Edge));
    newEdge->v1 = *v1;
    newEdge->v2 = *v2;
    newEdge->builtYear = builtYear;
    newEdge->length = length;

    listPushBack(graph->nodeTable[*v1]->edges, newEdge, NULL);
    listPushBack(graph->nodeTable[*v2]->edges, newEdge, NULL);

    return true; // TODO dodać gdzieś false
}

// chyba nie potrzebna ta funkcja
bool isInGraph(Graph *graph, char *label)
{
    return (hashtableGet(graph->labels, label) != NULL);
}

Edge *getEdge(Graph *graph, char *label1, char *label2)
{
    // TODO szukać w wierzchołku o mniejszym stopniu
    int *v1 = hashtableGet(graph->labels, label1);
    int *v2 = hashtableGet(graph->labels, label2);

    // jeśli nie ma któregoś wierzchołka to zwraca NULL(bo nie ma też krawędzi)

    Element *elem = graph->nodeTable[*v1]->edges->begin->next;

    while(elem->value != NULL && !(((Edge *)elem->value)->v1 == *v2 || ((Edge *)elem->value)->v2 == *v2))
    {
        elem = elem->next;
    }
    return elem->value;
}

void removeEdge(Graph *graph, char *label1, char *label2)
{
    int *v1 = hashtableGet(graph->labels, label1);
    int *v2 = hashtableGet(graph->labels, label2);

//    Egde *egde = getEdge(graph, label1, )

    Element *edge1 = graph->nodeTable[*v1]->edges->begin->next;
    Element *edge2 = graph->nodeTable[*v2]->edges->begin->next;

    while(edge1->value != NULL && !(((Edge *)edge1->value)->v1 == *v2 || ((Edge *)edge1->value)->v2 == *v2))
    {
        edge1 = edge1->next;
    }

    while(edge2->value != NULL && !(((Edge *)edge2->value)->v1 == *v1 || ((Edge *)edge2->value)->v2 == *v1))
    {
        edge2 = edge2->next;
    }

    free(edge1->value);
    listRemove(edge1);
    listRemove(edge2);
}