#include "graph.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>

// TODO long longi
// TODO przenieść odpowiednie opisy funkcji do graph.h

const int INF = INT_MAX/2; // TODO naprawić to
const int minYear = INT_MIN;
const int maxYear = INT_MAX;

Graph *newGraph()
{
    const int INIT_SIZE = 16;
    Graph *graph = calloc(1, sizeof(Graph));
    graph->nodes = calloc(INIT_SIZE, sizeof(Node*));
    graph->tableSize = INIT_SIZE;
    graph->nodeCount = 0;
    graph->ambiguous = newList(NULL);

    return graph;
}

OrientedEdge *newOrientedEdge(Edge *edge, int v)
{
    OrientedEdge *orientedEdge = calloc(1, sizeof(OrientedEdge));

    orientedEdge->edge = edge;
    orientedEdge->v = v;
    orientedEdge->isInPath = false;

    return orientedEdge;
}

void deleteOrientedEdge(OrientedEdge *edge)
{
    free(edge);
}

void deleteGraph(Graph *graph)
{
    for(int i=0; i<graph->nodeCount; i++)
    {
//        free(graph->nodes[i]->label);
        free(graph->nodes[i]->id);

        Element *elem = graph->nodes[i]->edges->begin->next;
        while(elem != graph->nodes[i]->edges->end)
        {
            Edge *edge = ((Edge*)elem->value);
            elem = elem->next;

            removeEdge(graph, edge->v1, edge->v2);

            free(edge);
        }
        deleteList(graph->nodes[i]->edges, 0);
    }
    free(graph->nodes);
    deleteList(graph->ambiguous, 0);
    free(graph);
}

/**
 * @brief Alokuje pamięć i inicjalizuje nowy obiekt Node.
 * @return Zwraca wskaźnik na nowo utworzony obiekt Node lub NULL, gdy
 * nie udało się zaalokować pamięci.
 */
Node *newNode() // TODO poprawić sprawdzanie czy się nie udało
{
    Node *node = calloc(1, sizeof(Node));
    if(node == NULL)
        return NULL;

    node->edges = newList(NULL);
    node->id = calloc(1,sizeof(int));
    node->visited = false;
//    node->label = label;
    return node;
}

// tworzy nowy obiekt Node i zwraca jego indeks w grafie. Jeśli nie udało się
// zaalokować pamięci zwraca NULL
/**
 * @brief Dodaje wierzchołek do grafu.
 * @param graph Graf, do którego ma zostać dodany wierzchołek.
 * @return Wskaźnik na id wierzchołka lub NULL, gdy nie udało się
 * zaalokować pamięci
 */
int *addNode(Graph *graph)
{
    graph->nodes[graph->nodeCount] = newNode();
    if(graph->nodes[graph->nodeCount] == NULL)
        return NULL;

    *graph->nodes[graph->nodeCount]->id = graph->nodeCount;
    graph->nodeCount++;

    if(graph->nodeCount >= graph->tableSize)
    {
        graph->tableSize *= 2;
        graph->nodes = realloc(graph->nodes, graph->tableSize*sizeof(Node));
        if(graph->nodes == NULL)
            return NULL;
    }
    return graph->nodes[graph->nodeCount-1]->id;
}

// dodaje krawędź pomiędzy istniejącymi wierzchołkami w grafie i zwraca true jeśli udało się ją stworzyć
// i false jeśli nie
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

    if(!listPushBack(graph->nodes[v1]->edges, newEdge, NULL))
        return false;
    if(!listPushBack(graph->nodes[v2]->edges, newEdge, NULL))
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

/**
 * @brief Szuka w grafie krawędzi pomiędzy @p v1 i @p v2
 * @param graph Przeszukiwany graf.
 * @param v1 id wierzchołka
 * @param v2 id wierzchołka
 * @return Wskaźnik na krawędź, lub NULL jeśli parametry są niepoprawne lub
 * w grafie nie ma krawędzi pomiędzy @p v1 i @p v2
 */
Edge *getEdge(Graph *graph, int v1, int v2)
{
    if(v1 == v2) // DO DOKUMENTACJI w grafie nie ma pęli
        return NULL; // dzięki funkcji otherNodeId ten warunek chyba niepotrzebny. Graf może obsługiwać pętle

    if(v1 >= graph->nodeCount || v2 >= graph->nodeCount) // któregoś z wierzchołków nie ma w grafie
        return NULL;

    // TODO dodać stopień wierzchołka, żeby szukać w wierzchołku o mniejszym stopniu

    foreach(it, graph->nodes[v1]->edges)
    {
        Edge *edge = it->value;
        if(otherNodeId(edge, v1) == v2) // znaleziono
            return edge;
    }

    return NULL; // nie znaleziono
}

/**
 * @brief Usuwa krawędź z grafu.
 * Nie zwalnia pamięci po usuniętej krawędzi.
 * @param graph Graf, z którego zostanie usunięta krawędź
 * @param v1 id wierzchołka grafu
 * @param v2 id wierzchołka grafu
 * @return @p true jeśli krawędź została poprawnie usunięta lub @p false, gdy
 * nie usunięto krawędzi(np. w grafie jej nie było).
 */
bool removeEdge(Graph *graph, int v1, int v2)
{
    if(getEdge(graph, v1, v2) == NULL) // w grafie nie ma takiej krawędzi
        return false;

    Element *edge1 = NULL;
    Element *edge2 = NULL;

    foreach(it, graph->nodes[v1]->edges)
    {
        if(otherNodeId(it->value, v1) == v2) // mamy krawędź
            edge1 = it;
    }

    foreach(it, graph->nodes[v2]->edges)
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
        foreach(it, graph->nodes[nodeId]->edges)
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



List *bestPath(Graph *graph, int v1, int v2)
{
    PriorityQueue *q = newPriorityQueue(compare);

    Path *bestPath = calloc(graph->nodeCount, sizeof(Path));
    Path *secondPath = calloc(graph->nodeCount, sizeof(Path));

    for(int i=0; i<graph->nodeCount; i++)
    {
          bestPath[i] = pathInit(INF, minYear, i, NULL);
          secondPath[i] = pathInit(INF, minYear, i, NULL);
    }

    bestPath[v1] = pathInit(0, maxYear, v1, NULL);
//    secondPath[v1] = pathInit(0, maxYear, v1, NULL);

    priorityQueuePush(q, &bestPath[v1]);

    while(!isEmpty(q))
    {
//        QueueElement *curr = priorityQueuePop(q);
        Path *n = priorityQueuePop(q);
        int curr = n->nodeId;

        if(graph->nodes[curr]->visited && curr != v1)
            continue;

        graph->nodes[curr]->visited = true;
//        Element *edges = graph->nodes[curr]->edges->begin->next;

        foreach(it, graph->nodes[curr]->edges)
        {
            Edge *edge = it->value;
            int other = otherNodeId(edge, curr);

            if (graph->nodes[other]->visited == false)
            {
                Path path1 = pathInit(bestPath[curr].dist + edge->length, min(bestPath[curr].year, edge->builtYear),
                                      other, newOrientedEdge(edge, curr));
                Path path2 = pathInit(secondPath[curr].dist + edge->length, min(secondPath[curr].year, edge->builtYear),
                                      other, newOrientedEdge(edge, curr));

                if (compare(&path1, &bestPath[other]) >= 0)
                {
                    secondPath[other] = bestPath[other];
                    bestPath[other] = path1;
                    priorityQueuePush(q, &bestPath[other]); // TODO wrzucać na kolejkę, tylko gdy poprawimy wynik
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

//        while(edges != graph->nodes[curr->nodeId]->edges->end)
//        {
//            int other;
//            if(curr->nodeId == ((Edge*)edges->value)->v1)
//                other = ((Edge*)edges->value)->v2;
//            else
//                other = ((Edge*)edges->value)->v1;
//
//            if(graph->nodes[other]->visited == false)
//            {
//                if(bestPath[other].dist > bestPath[curr->nodeId].dist + ((Edge*)edges->value)->length) // update bo odległość jest lepsza
//                {
//                    bestPath[other].dist = bestPath[curr->nodeId].dist + ((Edge*)edges->value)->length;
//                    bestPath[other].year = min(bestPath[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
//                    bestPath[other].parent = newOrientedEdge(edges->value, curr->nodeId);
////                    bestDistance[other].pathCount = bestDistance[curr->nodeId].pathCount;
//                    priorityQueuePush(q, &bestPath[other]);
//                }
//                else if(bestPath[other].dist == bestPath[curr->nodeId].dist + ((Edge*)edges->value)->length)
//                {
//                    if(bestPath[other].year < min(bestPath[curr->nodeId].year, ((Edge*)edges->value)->builtYear)) // update bo lepszy rok
//                    {
//                        bestPath[other].year = min(bestPath[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
//                        bestPath[other].parent = newOrientedEdge(edges->value, curr->nodeId);
////                        bestDistance[other].pathCount = bestDistance[curr->nodeId].pathCount;
//                        priorityQueuePush(q, &bestPath[other]);
//                    }
//                    if(bestPath[other].year == min(bestPath[curr->nodeId].year, ((Edge*)edges->value)->builtYear)) // update bo lepszy rok
////                        bestDistance[other].pathCount += bestDistance[curr->nodeId].pathCount;
//                }
//            }
//            edges = edges->next;
//        }
//    }

    for(int i=0; i<graph->nodeCount; i++)
        graph->nodes[i]->visited = false;


    // TODO zwalnianie
    if(secondPath[v2].dist == bestPath[v2].dist && secondPath[v2].year == bestPath[v2].year && bestPath[v2].dist != INF)
        return graph->ambiguous;

    if(bestPath[v2].parent == NULL) // brak ścieżki
        return NULL;

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

    deletePriorityQueue(q);
    for(int i=0; i<graph->nodeCount; i++)
        if(bestPath[i].parent != NULL && bestPath[i].parent->isInPath == false)
            free(bestPath[i].parent);
    free(bestPath);

    return path;
}