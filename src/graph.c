#include "graph.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <limits.h>

const int INF = INT_MAX;
const int minYear = -2000;
const int maxYear = 2;

Graph *newGraph()
{
    Graph *graph = calloc(1, sizeof(Graph));
//    graph->labels = newHashtable(16, NULL);
    graph->nodeTable = calloc(10, sizeof(Node*));
    graph->nodeCount = 0;

    return graph;
}

OrientedEdge *newOrientedEdge(Edge *edge, int v)
{
    OrientedEdge *orientedEdge = calloc(1, sizeof(OrientedEdge));

    orientedEdge->edge = edge;
    orientedEdge->v = v;

    return orientedEdge;
}


void deleteGraph(Graph *graph)
{
    // TODO deleteGraph
    return;
}

Node *newNode(const char *label)
{
    Node *node = calloc(1, sizeof(Node));
    if(node == NULL)
        return NULL;

    node->edges = newList(NULL);
    node->id = calloc(1,sizeof(int));
    node->visited = false;
    if(label == NULL)// todo potem chyba to usunąć
        node->label = "";
    else
        node->label = label;
    return node;
}

int *addNode(Graph *graph,
             const char *label) // TODO zastanowić się czy nie skopiować napisu przed dodaniem do hashtablicy. Bo ktoś może podmienić?
{
//    if(!isInGraph(graph, label))// TODO getHashtable + insert (2 przejścia niepotrzebnie)

    graph->nodeTable[graph->nodeCount] = newNode(label);
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
//bool isInGraph(Graph *graph, int v)
//{
//    // TODO poprawić tą funkcję albo usunąć jeśli niepotrzebna
//    return v < graph->nodeCount; // (hashtableGet(graph->labels, label) != NULL);
//}

Edge *getEdge(Graph *graph, int v1, int v2)
{
    // TODO szukać w wierzchołku o mniejszym stopniu

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

//    free(edge1->value);
    listRemove(edge1);
    listRemove(edge2);
}

int min(int a, int b)
{
    return (a<b?a:b);
}

// Zwraca listę wierzchołków będącą najkrótszą drogą pomiędzy v1 i v2. Jeśli istnieje wiele takich, to zwraca "najnowszą"
// Jeśli taka droga nie istnieje lub nie da się jej jednodznacznie ustalić, to zwraca NULL
List *bestPath(Graph *graph, int v1, int v2) // TODO pokminić czy long longi nie potrzebne
{
//    if(v1 >= graph->nodeCount || v2 >= graph->nodeCount)
//        return NULL;

    PriorityQueue *q = newPriorityQueue();

    QueueElement *bestDistance = calloc(graph->nodeCount, sizeof(QueueElement));

    for(int i=0; i<graph->nodeCount; i++)
    {
        bestDistance[i].dist = INF;
        bestDistance[i].year = minYear - 1;
        bestDistance[i].nodeId = i;
        bestDistance[i].parent = NULL;
    }

    bestDistance[v1].dist = 0;
    bestDistance[v1].year = maxYear + 1;

    priorityQueuePush(q, &bestDistance[v1]);

    while(!isEmpty(q))
    {
        QueueElement *curr = priorityQueuePop(q); // TODO niech curr to będzie int, id wierzchołka. Będzie łatwiej
        graph->nodeTable[curr->nodeId]->visited = true;
        Element *edges = graph->nodeTable[curr->nodeId]->edges->begin->next;

        while(edges != graph->nodeTable[curr->nodeId]->edges->end)
        {
            int other;
            if(curr->nodeId == ((Edge*)edges->value)->v1)
                other = ((Edge*)edges->value)->v2;
            else
                other = ((Edge*)edges->value)->v1;

            if(graph->nodeTable[other]->visited == false)
            {
                if(bestDistance[other].dist > bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length) // update bo odległość jest lepsza
                {
                    bestDistance[other].dist = bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length;
                    bestDistance[other].year = min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
                    bestDistance[other].parent = newOrientedEdge(edges->value, curr->nodeId);
                    priorityQueuePush(q, &bestDistance[other]);
                }
                else if(bestDistance[other].dist == bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length)
                {
                    if(bestDistance[other].year <= min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear)) // update bo lepszy rok
                    {
                        bestDistance[other].year = min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
                        bestDistance[other].parent = newOrientedEdge(edges->value, curr->nodeId);
                        priorityQueuePush(q, &bestDistance[other]);
                    }
                }
            }
            edges = edges->next;
        }
    }

    deletePriorityQueue(q);

    for(int i=0; i<graph->nodeCount; i++)
        graph->nodeTable[i]->visited = false;

    if(bestDistance[v2].parent == NULL)
        return NULL;

    // TODO dodać ifa na to czy jest jednoznaczna ścieżka

    List *path = newList(NULL); // TODO zastanowić się czy nie dać memory

    while(bestDistance[v2].parent != NULL)
    {
        listInsert(path->begin, bestDistance[v2].parent, NULL);
        v2 = bestDistance[v2].parent->v;
    }

    return path;
}