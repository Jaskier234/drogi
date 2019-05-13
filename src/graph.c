#include "graph.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>

// TODO long longi

const int INF = INT_MAX/2; // TODO naprawić to
const int minYear = INT_MIN;
const int maxYear = INT_MAX;

Graph *newGraph()
{
    Graph *graph = calloc(1, sizeof(Graph));

    if(graph == NULL)
        return NULL;

    graph->nodes = newVector();
    graph->nodeCount = 0;
    graph->ambiguous = newList(NULL);

    if(graph->nodes == NULL || graph->ambiguous == NULL)
    {
        deleteVector(graph->nodes);
        deleteList(graph->ambiguous, false);
        deleteGraph(graph);
        return NULL;
    }

    return graph;
}

void deleteGraph(Graph *graph)
{
    if(graph == NULL)
        return;

    for(int i=0; i<graph->nodeCount; i++)
    {
        Node *node = graph->nodes->tab[i];
        free(node->id);

        Element *elem = node->edges->begin->next;
        while(elem != node->edges->end)
        {
            Edge *edge = ((Edge*)elem->value);
            elem = elem->next;

            removeEdge(graph, edge->v1, edge->v2);

            free(edge);
        }
        deleteList(node->edges, false);
        free(node);
    }
    deleteVector(graph->nodes);
    deleteList(graph->ambiguous, 0);
    free(graph);
}

OrientedEdge *newOrientedEdge(Edge *edge, int v)
{
    OrientedEdge *orientedEdge = calloc(1, sizeof(OrientedEdge));

    if(orientedEdge == NULL)
        return NULL;

    orientedEdge->edge = edge;
    orientedEdge->v = v;
    orientedEdge->isInPath = false;

    return orientedEdge;
}

// chyba nie potrzebne
void deleteOrientedEdge(OrientedEdge *edge)
{
    free(edge);
}

/**
 * @brief Alokuje pamięć i inicjalizuje nowy obiekt Node.
 * @return Zwraca wskaźnik na nowo utworzony obiekt Node lub NULL, gdy
 * nie udało się zaalokować pamięci.
 */
Node *newNode()
{
    Node *node = calloc(1, sizeof(Node));

    if(node == NULL)
        return NULL;

    node->edges = newList(NULL);
    node->id = calloc(1,sizeof(int));
    node->visited = false;

    if(node->edges == NULL || node->id == NULL)
    {
        deleteList(node->edges, false);
        free(node->id);
        free(node);
        return NULL;
    }

    return node;
}

int *addNode(Graph *graph)
{
//    if(graph->nodeCount >= graph->tableSize)
//    {
//        graph->tableSize *= 2;
//        graph->nodes = realloc(graph->nodes, graph->tableSize*sizeof(Node));
//        if(graph->nodes == NULL)
//            return NULL;
//    }

    Node *node = newNode();
    if(node == NULL)
        return NULL;

    if(!vectorPushBack(graph->nodes, node)) // todo free node
        return NULL;

    *node->id = graph->nodeCount; // todo jedna linia
    graph->nodeCount++;

    return node->id;
}

bool addEdge(Graph *graph, int v1, int v2, int length, int builtYear)
{
    if(getEdge(graph, v1, v2) != NULL) // krawędź już jest w grafie
        return false;

    if(v1 >= graph->nodeCount || v2 >= graph->nodeCount) // któregoś z wierzchołków nie ma w grafie
        return false;

    Edge *newEdge = calloc(1, sizeof(Edge));

    if(newEdge == NULL)
        return false;

    newEdge->v1 = v1;
    newEdge->v2 = v2;
    newEdge->builtYear = builtYear;
    newEdge->length = length;

    Node *node1 = graph->nodes->tab[v1];
    if(!listPushBack(node1->edges, newEdge, NULL))
        return false;

    Node *node2 = graph->nodes->tab[v2];
    if(!listPushBack(node2->edges, newEdge, NULL))
        return false;

    return true;
}

/**
 * @brief Zwraca wierzchołek do którego prowadzi krawędź @p edge z wierzchołka @p v.
 * @param edge Rozpatrywana krawędź.
 * @param v id wierzchołka początkowego
 * @return Id wierzchołka do którego prowadzi krawędź.
 */
int otherNodeId(Edge *edge, int v)
{
    if(edge->v1 == v)
        return edge->v2;
    return edge->v1;
}

Edge *getEdge(Graph *graph, int v1, int v2)
{
    if(v1 == v2)
        return NULL;

    if(v1 >= graph->nodeCount || v2 >= graph->nodeCount) // któregoś z wierzchołków nie ma w grafie
        return NULL;

    // TODO dodać stopień wierzchołka, żeby szukać w wierzchołku o mniejszym stopniu

    Node *node = graph->nodes->tab[v1];
    foreach(it, node->edges)
    {
        Edge *edge = it->value;
        if(otherNodeId(edge, v1) == v2) // znaleziono
            return edge;
    }

    return NULL; // nie znaleziono
}

bool removeEdge(Graph *graph, int v1, int v2)
{
    if(getEdge(graph, v1, v2) == NULL) // w grafie nie ma takiej krawędzi
        return false;

    Element *edge1 = NULL;
    Element *edge2 = NULL;

    Node *node1 = graph->nodes->tab[v1];
    foreach(it, node1->edges)
    {
        if(otherNodeId(it->value, v1) == v2) // mamy krawędź
            edge1 = it;
    }

    Node *node2 = graph->nodes->tab[v2];
    foreach(it, node2->edges)
    {
        if(otherNodeId(it->value, v2) == v1)
            edge2 = it;
    }

    // krawędź występuje grafie, więc edge1 i edge2 nie mogą być równe NULL
    assert(edge1 != NULL);
    assert(edge2 != NULL);

    if(!listRemove(edge1) || !listRemove(edge2))
        return false;

    return true;
}

int min(int a, int b)
{
    return (a<b?a:b);
}

void printGraph(Graph *graph)
{
    for(int nodeId=0; nodeId < graph->nodeCount; nodeId++)
    {
        printf("%d: ", nodeId);
        Node *node = graph->nodes->tab[nodeId];
        foreach(it, node->edges)
        {
            Edge *edge = it->value;
            printf("%d ", otherNodeId(edge, nodeId));
        }
        printf("\n");
    }
}

// Zwraca listę wierzchołków będącą najkrótszą drogą pomiędzy v1 i v2. Jeśli istnieje wiele takich, to zwraca "najnowszą"
// Jeśli taka droga nie istnieje lub nie da się jej jednodznacznie ustalić, to zwraca NULL

typedef struct Path
{
    int dist;
    int year;
    int nodeId;
    OrientedEdge *parent;
} Path;

Path pathInit(int dist, int year, int node, OrientedEdge *parent)
{
    Path p;
    p.dist = dist;
    p.year = year;
    p.nodeId = node;
    p.parent = parent;

    return p;
}

// 1 - jeśli path1 jest lepsza od path2
// 0 - takie same
int compare(void *a, void *b)
{
    Path *path1 = a;
    Path *path2 = b;

    if(path1->dist == path2->dist)
    {
        if(path1->year == path2->year)
            return 0;
        if(path1->year < path2->year)
            return -1;
        return 1;
    }
    return ((path1->dist < path2->dist)?(1):(-1)); // 1 - path1 lepsza
}


// NULL gdy problemy z pamiecią
List *bestPath(Graph *graph, int v1, int v2)
{
    PriorityQueue *q = newPriorityQueue(compare);

    List *orientedEdges = newList(NULL);

    Path *bestPath = calloc(graph->nodeCount, sizeof(Path));
    Path *secondPath = calloc(graph->nodeCount, sizeof(Path));

    if(bestPath == NULL || secondPath == NULL)
    {
        free(bestPath);
        free(secondPath);
        deletePriorityQueue(q);
        return NULL;
    }

    for(int i=0; i<graph->nodeCount; i++)
    {
          bestPath[i] = pathInit(INF, minYear, i, NULL);
          secondPath[i] = pathInit(INF, minYear, i, NULL);
    }

    bestPath[v1] = pathInit(0, maxYear, v1, NULL);

    // TODO sprawdzanie czy priorityQueuePush się powiodło

    priorityQueuePush(q, &bestPath[v1]);

    while(!isEmpty(q))
    {
        Path *n = priorityQueuePop(q);
        int curr = n->nodeId;

        Node *currentNode = graph->nodes->tab[curr];

        if(currentNode->visited && curr != v1)
            continue;

        currentNode->visited = true;

        foreach(it, currentNode->edges)
        {
            Edge *edge = it->value;
            int other = otherNodeId(edge, curr);
            Node *otherNode = graph->nodes->tab[other];

            if (otherNode->visited == false)
            {
                Path path1 = pathInit(bestPath[curr].dist + edge->length, min(bestPath[curr].year, edge->builtYear),
                                      other, newOrientedEdge(edge, curr));
                Path path2 = pathInit(secondPath[curr].dist + edge->length, min(secondPath[curr].year, edge->builtYear),
                                      other, newOrientedEdge(edge, curr));

                listPushBack(orientedEdges, path1.parent, NULL);
                listPushBack(orientedEdges, path2.parent, NULL);

                if (compare(&path1, &bestPath[other]) >= 0)
                {
                    secondPath[other] = bestPath[other];
                    bestPath[other] = path1;
                    priorityQueuePush(q, &bestPath[other]);
                }
                else if (compare(&path1, &secondPath[other]) >= 0)
                    secondPath[other] = path1;

//                if (compare(&path2, &bestPath[other]) >= 0)
//                {
//                    secondPath[other] = bestPath[other];
//                    bestPath[other] = path2;
//                    priorityQueuePush(q, &bestPath[other]);
//                }
                if (compare(&path2, &secondPath[other]) >= 0)
                    secondPath[other] = path2;

            }
        }
    }

    for(int i=0; i<graph->nodeCount; i++)
    {
        Node *node = graph->nodes->tab[i];
        node->visited = false;
    }

    deletePriorityQueue(q);

    if(secondPath[v2].dist == bestPath[v2].dist && secondPath[v2].year == bestPath[v2].year && bestPath[v2].dist != INF)
    {
        free(bestPath);
        free(secondPath);
        deleteList(orientedEdges, true);
        return graph->ambiguous;
    }

    if(bestPath[v2].parent == NULL) // brak ścieżki
    {
        free(bestPath);
        free(secondPath);
        deleteList(orientedEdges, true);
        return NULL;
    }

//    if(bestPath[v2].pathCount != 1 || bestPath[v2].parent == NULL)
//    {
//        deletePriorityQueue(q);
//        for(int i=0; i<graph->nodeCount; i++)
//            if(bestPath[i].parent != NULL && bestPath[i].parent->isInPath == false)
//                free(bestPath[i].parent);
//        free(bestPath);
//        return NULL;
//    }

    List *path = newList(NULL);

    while(bestPath[v2].parent != NULL)
    {
        listInsert(path->begin, bestPath[v2].parent, NULL);
        bestPath[v2].parent->isInPath = true;
        v2 = bestPath[v2].parent->v;
    }

//    for(int i=0; i<graph->nodeCount; i++)
//        if(bestPath[i].parent != NULL && bestPath[i].parent->isInPath == false)
//            free(bestPath[i].parent);

    foreach(it, orientedEdges)
    {
        OrientedEdge *edge = it->value;
        if(edge != NULL && edge->isInPath == false)
            free(edge);
    }
    deleteList(orientedEdges, false);

    free(bestPath);
    free(secondPath);

    return path;
}