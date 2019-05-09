#include "graph.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <stdio.h>

// TODO long longi
// TODO przenieść odpowiednie opisy funkcji do graph.h

const int INF = INT_MAX;
const int minYear = INT_MIN;
const int maxYear = INT_MAX;

Graph *newGraph()
{
    const int INIT_SIZE = 16;
    Graph *graph = calloc(1, sizeof(Graph));
    graph->nodes = calloc(INIT_SIZE, sizeof(Node*));
    graph->tableSize = INIT_SIZE;
    graph->nodeCount = 0;

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
List *bestPath(Graph *graph, int v1, int v2)
{
    PriorityQueue *q = newPriorityQueue();

    QueueElement *bestDistance = calloc(graph->nodeCount, sizeof(QueueElement));

    for(int i=0; i<graph->nodeCount; i++)
    {
        bestDistance[i].dist = INF;
        bestDistance[i].year = minYear;
        bestDistance[i].nodeId = i;
        bestDistance[i].parent = NULL;
        bestDistance[i].pathCount = 0;
    }

    bestDistance[v1].dist = 0;
    bestDistance[v1].year = maxYear;
    bestDistance[v1].pathCount = 1;

    priorityQueuePush(q, &bestDistance[v1]);

    while(!isEmpty(q))
    {
        QueueElement *curr = priorityQueuePop(q);
        graph->nodes[curr->nodeId]->visited = true;
        Element *edges = graph->nodes[curr->nodeId]->edges->begin->next;

        while(edges != graph->nodes[curr->nodeId]->edges->end)
        {
            int other;
            if(curr->nodeId == ((Edge*)edges->value)->v1)
                other = ((Edge*)edges->value)->v2;
            else
                other = ((Edge*)edges->value)->v1;

            if(graph->nodes[other]->visited == false)
            {
                if(bestDistance[other].dist > bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length) // update bo odległość jest lepsza
                {
                    bestDistance[other].dist = bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length;
                    bestDistance[other].year = min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
                    bestDistance[other].parent = newOrientedEdge(edges->value, curr->nodeId);
                    bestDistance[other].pathCount = bestDistance[curr->nodeId].pathCount;
                    priorityQueuePush(q, &bestDistance[other]);
                }
                else if(bestDistance[other].dist == bestDistance[curr->nodeId].dist + ((Edge*)edges->value)->length)
                {
                    if(bestDistance[other].year < min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear)) // update bo lepszy rok
                    {
                        bestDistance[other].year = min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear);
                        bestDistance[other].parent = newOrientedEdge(edges->value, curr->nodeId);
                        bestDistance[other].pathCount = bestDistance[curr->nodeId].pathCount;
                        priorityQueuePush(q, &bestDistance[other]);
                    }
                    if(bestDistance[other].year == min(bestDistance[curr->nodeId].year, ((Edge*)edges->value)->builtYear)) // update bo lepszy rok
                        bestDistance[other].pathCount += bestDistance[curr->nodeId].pathCount;
                }
            }
            edges = edges->next;
        }
    }

    for(int i=0; i<graph->nodeCount; i++)
        graph->nodes[i]->visited = false;

    if(bestDistance[v2].pathCount != 1 || bestDistance[v2].parent == NULL)
    {
        deletePriorityQueue(q);
        for(int i=0; i<graph->nodeCount; i++)
            if(bestDistance[i].parent != NULL && bestDistance[i].parent->isInPath == false)
                free(bestDistance[i].parent);
        free(bestDistance);
        return NULL;
    }

    List *path = newList(NULL);

    while(bestDistance[v2].parent != NULL)
    {
        listInsert(path->begin, bestDistance[v2].parent, NULL);
        bestDistance[v2].parent->isInPath = true;
        v2 = bestDistance[v2].parent->v;
    }

    deletePriorityQueue(q);
    for(int i=0; i<graph->nodeCount; i++)
        if(bestDistance[i].parent != NULL && bestDistance[i].parent->isInPath == false)
            free(bestDistance[i].parent);
    free(bestDistance);

    return path;
}