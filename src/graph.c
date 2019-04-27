#include "graph.h"

#include <stdlib.h>

Graph *newGraph()
{
    Graph *graph = calloc(1, sizeof(Graph*));
//    graph->labels = newHashtable(16, NULL);
    graph->nodeTable = calloc(10, sizeof(Node*));
    graph->nodeCount = 0;

    return graph;
}

void deleteGraph(Graph *graph)
{
    // TODO deleteGraph
    return;
}

Node *newNode()
{
    Node *node = calloc(1, sizeof(Node));
    if(node == NULL)
        return NULL;

    node->edges = newList(NULL);
    node->id = calloc(1,sizeof(int));
    return node;
}

int *addNode(Graph *graph) // TODO zastanowić się czy nie skopiować napisu przed dodaniem do hashtablicy. Bo ktoś może podmienić
{
//    if(!isInGraph(graph, label))// TODO getHashtable + insert (2 przejścia niepotrzebnie)

    graph->nodeTable[graph->nodeCount] = newNode();
    if(graph->nodeTable[graph->nodeCount] == NULL)
        return NULL;

    *graph->nodeTable[graph->nodeCount]->id = graph->nodeCount;
    graph->nodeCount++;

    // TODO resize nodetable
    return graph->nodeTable[graph->nodeCount-1]->id;
}

// dodaje krawędź pomiędzy istniejącymi wierzchołkami w grafie i zwraca ją
bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear)
{
    // TODO kilka zapytań do hashmapy o to samo(może jakaś fajna funkcja)
    // TODO sprawdzić czy nie istnieje już taka krawędź
    if(getEdge(graph, v1, v2) != NULL) // jeśli krawędź już jest w grafie to nic nie rób(w module map)
        return NULL;

    if(v1 >= graph->nodeCount || v2 >= graph->nodeCount) // któregoś z wierzchołków nie ma w grafie
        return NULL;

//    addNode(graph, v1);
//    addNode(graph, v2);

//    int *v1 = hashtableGet(graph->labels, v1);
//    int *v2 = hashtableGet(graph->labels, v2);

    Edge *newEdge = calloc(1, sizeof(Edge));

    if(newEdge == NULL)
        return false;

    newEdge->v1 = v1;
    newEdge->v2 = v2;
    newEdge->builtYear = builtYear;
    newEdge->length = length;

    listPushBack(graph->nodeTable[v1]->edges, newEdge, NULL);
    listPushBack(graph->nodeTable[v2]->edges, newEdge, NULL);

    return true; // TODO dodać gdzieś NULL jeśli się coś nie uda
}

// chyba nie potrzebna ta funkcja
bool isInGraph(Graph *graph, int v)
{
    // TODO poprawić tą funkcję albo usunąć jeśli niepotrzebna
    return v < graph->nodeCount; // (hashtableGet(graph->labels, label) != NULL);
}

Edge *getEdge(Graph *graph, int v1, int v2)
{
    // TODO szukać w wierzchołku o mniejszym stopniu
//    int *v1 = hashtableGet(graph->labels, v1);
//    int *v2 = hashtableGet(graph->labels, v2);

    // jeśli nie ma któregoś wierzchołka to zwraca NULL(bo nie ma też krawędzi)

    Element *elem = graph->nodeTable[v1]->edges->begin->next;

    while(elem->value != NULL && !(((Edge *)elem->value)->v1 == v2 || ((Edge *)elem->value)->v2 == v2))
    {
        elem = elem->next;
    }
    return elem->value;
}

void removeEdge(Graph *graph, int v1, int v2)
{
//    Egde *egde = getEdge(graph, label1, )

    Element *edge1 = graph->nodeTable[v1]->edges->begin->next;
    Element *edge2 = graph->nodeTable[v2]->edges->begin->next;

    while(edge1->value != NULL && !(((Edge *)edge1->value)->v1 == v2 || ((Edge *)edge1->value)->v2 == v2))
    {
        edge1 = edge1->next;
    }

    while(edge2->value != NULL && !(((Edge *)edge2->value)->v1 == v1 || ((Edge *)edge2->value)->v2 == v1))
    {
        edge2 = edge2->next;
    }

    free(edge1->value);
    listRemove(edge1);
    listRemove(edge2);
}